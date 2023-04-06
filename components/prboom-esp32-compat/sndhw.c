//Audio driver. Because the S3 doesn't have a DAC, we use PDM (delta-sigma) output,
//combined with an external analog Sallen-Key lowpass filter. This should actually
//give a better output than the 8-bit DAC in the ESP32.
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/i2s_std.h"
#include <driver/gpio.h>
#include "soc/rtc.h"
#include "soc/rtc_periph.h"
#include "esp_log.h"
#include "sndhw.h"
#include "es8311.h"
#include "esp_check.h"

//Implement a lock, as we don't want to run the sound callback while
//the main code is sending commands to it as well.
static SemaphoreHandle_t audio_mux;

#define TAG "audio"

snd_cb_t audio_cb;

void sndhw_lock() {
	xSemaphoreTake(audio_mux, portMAX_DELAY);
}

void sndhw_unlock() {
	xSemaphoreGive(audio_mux);
}

//make this equal to the imf tick rate so the music sounds good
#define SND_CHUNKSZ 560


static i2s_chan_handle_t tx_channel;

void audio_task(void *arg) {
	int16_t snd_in[SND_CHUNKSZ]={0};
	while (1) {
		//Get a chunk of audio data from the source...
		xSemaphoreTake(audio_mux, portMAX_DELAY);
		audio_cb((uint8_t*)snd_in, sizeof(snd_in)/2);
		xSemaphoreGive(audio_mux);
		//send it
		int bytes_written;
		ESP_ERROR_CHECK(i2s_channel_write(tx_channel, snd_in, sizeof(snd_in), &bytes_written, portMAX_DELAY));
	}
}


void sndhw_init(int samprate, snd_cb_t cb) {
	i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
	chan_cfg.auto_clear = true;
	ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_channel, NULL));
	i2s_std_config_t i2s_cfg = {
		.clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(samprate),
		.slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
		.gpio_cfg = {
			.mclk = 2,
			.bclk = 17,
			.ws = 47,
			.dout = 15,
			.din = 16,
			.invert_flags = {
				.mclk_inv = false,
				.bclk_inv = false,
				.ws_inv = false,
			}
		}
	};
	ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_channel, &i2s_cfg));
	ESP_ERROR_CHECK(i2s_channel_enable(tx_channel));

//Note: the following assumes the touchscreen driver already initialized I2C peripheral 0 to the
//ESP32-S3-BOX internal I2C bus.

	es8311_handle_t es8311=es8311_create(0, ES8311_ADDRRES_0);
	assert(es8311);

	const es8311_clock_config_t clk_cfg={
		.sample_frequency=samprate
	};
	ESP_ERROR_CHECK(es8311_init(es8311, &clk_cfg, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16));

	es8311_voice_volume_set(es8311, 60, NULL);

	const gpio_config_t io_conf = {
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask = BIT64(46),
	};
	gpio_config(&io_conf);
	gpio_set_level(46, 1);

	audio_mux=xSemaphoreCreateMutex();
	audio_cb=cb;
	xTaskCreatePinnedToCore(&audio_task, "snd", 16*1024, NULL, 3, NULL, 1);
	ESP_LOGI(TAG, "Audio inited.");
}
