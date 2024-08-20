#include "../inc/display.h"
#include <stdio.h>
#include <esp_task_wdt.h>

void app_main(void)
{   
    esp_task_wdt_deinit();
    printf("Initializing...\n");
    InitDisplay();
    printf("DONE\n");
}