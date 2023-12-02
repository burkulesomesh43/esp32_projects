/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/rtos_queue/main/main.c
    
  @brief
    rtos queue feature main file

  @details 
	  send data between two tasks.

  @note
    https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api

  @author
    Somesh Burkule

  @date
    01/12/2023
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* Initialise Queue Handle */
QueueHandle_t  queue_handle = NULL;

void consumer_task( void *pvParameter )
{
    printf( "consumer task  started\n" );
    int data_to_rec;

    while( 1 )
    {
        /* Receive data added in queue */
        xQueueReceive( queue_handle, &data_to_rec, (TickType_t )( 100/portTICK_PERIOD_MS ) ); 
        printf( "value received on queue: %d \n", data_to_rec );
        vTaskDelay( 500/portTICK_PERIOD_MS );
    }
}

void producer_task(void *pvParameter)
{
    printf( "producer task  started\n" );
    int data_to_send = 1; 

    while( 1 )
    {
         printf( "value sent on queue: %d \n", data_to_send );
         xQueueSend( queue_handle, ( void * )&data_to_send, (TickType_t )0 );
         data_to_send++;

         vTaskDelay( 1000/portTICK_PERIOD_MS ); 
    }
}

void app_main()
{	
    /* Create queue to store 20 values */
    queue_handle = xQueueCreate( 100, sizeof(int) );

    vTaskDelay( 100/portTICK_PERIOD_MS );

    /* check if queue is created successfully */
    if( queue_handle != NULL )
    {
        printf( "Queue is created\n" );

        xTaskCreate( &producer_task, "producer_task", 2048, NULL, 5, NULL );
        xTaskCreate( &consumer_task, "consumer_task", 2048, NULL, 5, NULL );   
    }
    else
    {
        printf( "Queue creation failed" );
    }    
}
