#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#undef false
#undef true
#include "i_system.h"


void doomEngineTask(void *pvParameters)
{
    char const *argv[]={"doom", NULL};
    doom_main(1, argv);
}

void app_main()
{
    xTaskCreatePinnedToCore(&doomEngineTask, "doomEngine", 2048, NULL, 5, NULL, 0);
}
