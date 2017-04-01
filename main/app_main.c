#include "esp_attr.h"

#include "rom/cache.h"
#include "rom/ets_sys.h"
#include "rom/spi_flash.h"
#include "rom/crc.h"

#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "soc/io_mux_reg.h"
#include "soc/efuse_reg.h"
#include "soc/rtc_cntl_reg.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdlib.h>
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_partition.h"

#undef false
#undef true
#include "i_system.h"

#include "spi_lcd.h"

#include "memaccess_hist.h"

unsigned char *doom1waddata;

extern void Cache_Flush(int);

void doomEngineTask(void *pvParameters)
{
    char const *argv[]={"doom","-cout","ICWEFDA", NULL};
    doom_main(3, argv);
}

void app_main()
{
	int i;
	const esp_partition_t* part;
	spi_flash_mmap_handle_t hdoomwad;
	esp_err_t err;

	part=esp_partition_find_first(66, 6, NULL);
	if (part==0) printf("Couldn't find bootrom part!\n");
	err=esp_partition_mmap(part, 0, 3114091, SPI_FLASH_MMAP_DATA, (const void**)&doom1waddata, &hdoomwad);
	if (err!=ESP_OK) printf("Couldn't map bootrom part!\n");

//	printf("Loaded wad okay, addr=%p\n", doom1waddata);
//	for (i=0; i<16; i++) printf("%x ", doom1waddata[i]);
	
	memaccess_history_init(0x0);

	spi_lcd_init();
    xTaskCreatePinnedToCore(&doomEngineTask, "doomEngine", 32480, NULL, 5, NULL, 0);
}
