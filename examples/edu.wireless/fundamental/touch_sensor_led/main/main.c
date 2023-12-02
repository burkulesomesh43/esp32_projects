/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/touch_sensor_led/main/main.c
  @brief
    Inbuilt esp32 capacitive touch sensor source code.file

  @details
    The capacitive touch sensor from esp32 is used to read touch points from gpio
    pins 0 to 9.On the built in led if the touch points got any touch 

  @note
    API Refernces:
    https://docs.espressif.com/projects/esp-idf/en/v4.3.1/esp32/api-reference/peripherals/touch_pad.html

  @author
    Somesh Burkule

  @date
    01/12/2023
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "driver/gpio.h"


#define TOUCH_THRESH_NO_USE   (0)
#define TP_NUM                (0)   /* Range : 0 - 9 */
#define LED                   (2)
#define LED_ON                (1)
#define LED_OFF               (0)
#define THRESHOLD             (500)


/*
  Task to read sensor processing stage.
 */
static void touch_sensor_read_task( void *pvParameter )
{
  uint16_t touch_value;
  printf( "Touch Sensor normal mode read\n\n" );

  while ( 1 ) 
  {
    touch_pad_read(TP_NUM, &touch_value);
    printf( "VALUE OF TOUCH POINT %d:[%4d]\n ", TP_NUM, touch_value );
    if( touch_value < THRESHOLD )
    {
      gpio_set_level( LED, LED_ON );
      printf( "touched\n");
    }
    else
    {
      gpio_set_level( LED, LED_OFF);
      printf( "untouched\n" );
    }
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

void gpio_setup(void)
{
  gpio_reset_pin( LED );/* clear the gpio */
  gpio_set_direction( LED, GPIO_MODE_OUTPUT );/* set the gpio as output */
}

void app_main(void)
{
  gpio_setup(); 
  /* Initialize touch pad peripheral. */
  ESP_ERROR_CHECK(touch_pad_init());
  /* 
  * Sensor configuration
  * High reference valtage: 2.7V - 1V = 1.7V
  * Low reference voltage : 0.5
  * The larger the range, the larger the pulse count value. 
  */
  touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
  touch_pad_config(TP_NUM, TOUCH_THRESH_NO_USE);
  
  /* Start task to read values from touch sensor */
  xTaskCreate(&touch_sensor_read_task, "touch_sensor_read_task", 2048, NULL, 5, NULL);
}
