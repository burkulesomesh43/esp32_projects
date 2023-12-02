/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/deep_sleep/main/main.c
    
  @brief
    deep sleep source code.file

  @details 
	The deep sleep mode is used to save power in battery operated application's

  @note

  @author
    Somesh Burkule

  @date
    01/12/2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"

#define CPU_WAKEUP_TIME_SEC     20

/* struct to define time value */
static RTC_DATA_ATTR struct timeval sleep_enter_time;


void app_main(void)
{
    struct timeval now;

    /* get current time */
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    /* read the cpu wakeup reason */
    int wakeup_cause = esp_sleep_get_wakeup_cause();

    switch ( wakeup_cause ) 
    {
        case ESP_SLEEP_WAKEUP_TIMER: 
        {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }

        default:
            printf("Not a deep sleep reset\n");
            break;
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    printf("Enabling timer wakeup, %ds\n", CPU_WAKEUP_TIME_SEC);
    esp_sleep_enable_timer_wakeup( CPU_WAKEUP_TIME_SEC * 1000000 );

#if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    printf("Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);

    /* start the deep sleep mode */
    esp_deep_sleep_start();
}

