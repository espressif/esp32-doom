// Copyright 2016-2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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


extern void jsInit();


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
	if (part==0) printf("Couldn't find wad part!\n");

	spi_lcd_init();
	jsInit();
	xTaskCreatePinnedToCore(&doomEngineTask, "doomEngine", 22480, NULL, 5, NULL, 0);
}
