/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/basic/main/main.c

  @brief
    basic functions to use 

  @details 

  @note

  @author
    Somesh Burkule

  @date
    01/12/23
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_mac.h"

/* Use tag to print logs */
#define TAG "INTERRUPT"

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

char task_list[250] = {0};
uint8_t mac_addr[6] = {0};

void task2( void *pvParameter )
{
  ESP_LOGI(TAG, "task2  started" );

  while( 1 )
  {     
    vTaskDelay( 1000/portTICK_PERIOD_MS );
  }
}

void task1(void *pvParameter)
{
  ESP_LOGI(TAG, "task1 started" );

  while( 1 )
  {
    vTaskDelay( 1000/portTICK_PERIOD_MS ); 
  }
}

void app_main()
{	
  /* To get reset reason of esp32 */
  ESP_LOGI( TAG, "[APP] Reset Reason: %d", esp_reset_reason() );

  /* To get available heap memory in esp32 */
  ESP_LOGI( TAG, "[APP] Free memory: %lu bytes", esp_get_free_heap_size() );

  /* To get esp-idf version */
  ESP_LOGI( TAG, "[APP] IDF version: %s", esp_get_idf_version() );

  /* To get Manufacturer Address of esp32 */
  esp_read_mac( mac_addr, ESP_MAC_WIFI_STA );
  ESP_LOGI( TAG, "[APP] WIFI MAC: %X:%X:%X:%X:%X:%X", mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5] );

  xTaskCreate( task1, "task1", 2048, NULL, 5, &task1Handle );
  xTaskCreate( task2, "task2", 2048, NULL, 5, &task2Handle );

  vTaskDelay( 1000/portTICK_PERIOD_MS );

  /* To get list of task and data associated with them */
  vTaskList( task_list );
  ESP_LOGI( TAG, "*******************************" );
  ESP_LOGI( TAG, "%s", task_list );
  ESP_LOGI( TAG, "*******************************" );
}
