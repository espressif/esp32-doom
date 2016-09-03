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
#include "soc/timers_reg.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <psram.h>

#undef false
#undef true
#include "i_system.h"


#define DOOMWAD_FLASHPOS 0x100000
#define DOOMWAD_MMUPOS (0x3f400000+DOOMWAD_FLASHPOS-0x20000)

unsigned char *doom1waddata=(unsigned char *)(DOOMWAD_MMUPOS);

extern void Cache_Flush(int);


void doomEngineTask(void *pvParameters)
{
    char const *argv[]={"doom", NULL};
    doom_main(1, argv);
}

#if 0
//Assumes Doom1.wad (well, the stripped down version) is located at 0xC0000 in flash. ToDo: add partition for it
//NOT USED ANYMORE. Doom now uses a custom bootloader that maps the entire memory instead of just the bit the app requests.
int IRAM_ATTR remapDoomWad(int zero, int one, int sixtyfour, int paddr, int vaddr, int len, int sramaddr) 
{
    int ret;
    Cache_Read_Disable(zero);
    Cache_Read_Disable(one);
    Cache_Flush(zero);
    Cache_Flush(one);
    ret=cache_flash_mmu_set( zero, zero, vaddr, paddr, sixtyfour, len );
    ret=cache_flash_mmu_set( one , zero, vaddr, paddr, sixtyfour, len );
    cache_sram_mmu_set(zero, zero, sramaddr,zero,sixtyfour/2,sixtyfour*2);
    cache_sram_mmu_set(one, zero, sramaddr,zero,sixtyfour/2,sixtyfour*2);
    REG_CLR_BIT( PRO_CACHE_CTRL1_REG, (DPORT_PRO_CACHE_MASK_IRAM0) | (DPORT_PRO_CACHE_MASK_IRAM1 & 0) | (DPORT_PRO_CACHE_MASK_IROM0 & 0) | DPORT_PRO_CACHE_MASK_DROM0 | DPORT_PRO_CACHE_MASK_DRAM1 );
    REG_CLR_BIT( APP_CACHE_CTRL1_REG, (DPORT_APP_CACHE_MASK_IRAM0) | (DPORT_APP_CACHE_MASK_IRAM1 & 0) | (DPORT_APP_CACHE_MASK_IROM0 & 0) | DPORT_APP_CACHE_MASK_DROM0 | DPORT_APP_CACHE_MASK_DRAM1 );
    Cache_Read_Enable( zero );
    Cache_Read_Enable( one );
	psram_enable(PsramCache_F80M_S80M);
	
//	asm("break.n 1");
    return ret;
}
#endif

void app_main()
{

//    int ret;
	int x;
//    ret=remapDoomWad(0, 1, 64, 0x20000, 0x3f400000, (1024*1024*4)/(64*1024), 0x3f800000);
//    ets_printf("remapDoomWad: %x\n", ret);
    for (x=0; x<32; x++) ets_printf("%02x ", doom1waddata[x]);
    ets_printf("\n");

    xTaskCreatePinnedToCore(&doomEngineTask, "doomEngine", 30480, NULL, 5, NULL, 0);
}
