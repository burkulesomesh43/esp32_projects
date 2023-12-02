/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/blink/main/main.c
    
  @brief
    Inbuilt esp32 LED source code.file

  @details
    The LED connected to development kit is used to run this code. 

  @note
    API Refernces:
    https://docs.espressif.com/projects/esp-idf/en/v4.3.1/esp32/api-reference/peripherals/gpio.html?highlight=gpio_reset_pin#

  @author
    Somesh Burkule

  @date
    01/12/2023
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


#define BLINK_GPIO  2
#define LED_ON      1
#define LED_OFF     0

void app_main(void)
{
    /* Clear the GPIO */
    gpio_reset_pin(BLINK_GPIO);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while(1) 
    {
        /* Blink off (output low) */
        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, LED_OFF);

        /* Add delay to turn on LED for that time */
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        /* Blink on (output high) */
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, LED_ON);

        /* Add delay to turn on LED for that time */
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
