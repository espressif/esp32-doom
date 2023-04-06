#include "driver/i2c.h"
#include "esp_lcd_touch_tt21100.h"


static esp_lcd_touch_handle_t tp;

void tsJsInputInit() {
	esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_TT21100_CONFIG();
	i2c_config_t i2c_conf = {
		.mode = I2C_MODE_MASTER, // I2C LCD is a master node
		.sda_io_num = 8,
		.scl_io_num = 18,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 100000,
	};
	ESP_ERROR_CHECK(i2c_param_config(0, &i2c_conf));
	ESP_ERROR_CHECK(i2c_driver_install(0, I2C_MODE_MASTER, 0, 0, 0));
	esp_lcd_panel_io_handle_t io_handle = NULL;
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)0, &io_config, &io_handle));

	esp_lcd_touch_config_t tp_cfg = {
		.x_max = 320,
		.y_max = 240,
		.rst_gpio_num = -1,
		.int_gpio_num = -1,
		.levels = {
			.reset = 0,
			.interrupt = 0,
		},
		.flags = {
			.swap_xy = 0,
			.mirror_x = 1,
			.mirror_y = 0,
		},
	};
	
	ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_tt21100(io_handle, &tp_cfg, &tp));
}

typedef struct {
	int x;
	int y;
	int b;
} v_but_t;

#define BUT_R 32

const v_but_t v_but[]={
	{64, 120-64, 0x10}, //up
	{64, 120+64, 0x40}, //down
	{32, 120, 0x80}, //left
	{32+64, 120, 0x20}, //right
	{320-32, 120+64 , 0x4000},
	{320-32, 120, 0x2000},
	{320+32, 120-64, 0x1000},
	{0,0,0}
};


int tsJsInputGet() {
    uint16_t touch_x[8];
    uint16_t touch_y[8];
    uint16_t touch_strength[8];
    uint8_t touch_cnt = 0;

    esp_lcd_touch_read_data(tp);
	int btn=0xffff;
    if (esp_lcd_touch_get_coordinates(tp, touch_x, touch_y, touch_strength, &touch_cnt, 8)) {
		for (int i=0; i<touch_cnt; i++) {
			if (touch_strength[i]>0) {
				int j=0;
				while (v_but[j].b!=0) {
					if (touch_x[i]>v_but[j].x-BUT_R && touch_x[i]<v_but[j].x+BUT_R &&
							touch_y[i]>v_but[j].y-BUT_R && touch_y[i]<v_but[j].y+BUT_R) {
						btn&=~v_but[j].b;
					}
					j++;
				}
			}
		}
	}
	return btn;
}
