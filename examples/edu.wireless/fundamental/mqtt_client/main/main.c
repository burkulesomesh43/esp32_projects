/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/mqtt_client/main/main.c

  @brief
    mqtt client source code.file

  @details 

  @note

  @author
    Somesh Burkule

  @date
    01/12/2023
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"


#define WIFI_SSID      "edu.wireless"
#define WIFI_PASS      "IoT@1234"
#define BROKER_URL     "mqtt://test.mosquitto.org"

/* Macro used to print logs on monitor */
#define TAG  "MQTT_CLIENT"

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

bool got_ip;

static void mqtt_event_handler( void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data )
{
    ESP_LOGD( TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id );
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ( ( esp_mqtt_event_id_t )event_id )
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI( TAG, "MQTT_EVENT_CONNECTED" );
            msg_id = esp_mqtt_client_publish( client, "/topic/qos0", "edu.wireless", 0, 0, 0 );
            ESP_LOGI( TAG, "sent publish successful, msg_id=%d", msg_id );

            msg_id = esp_mqtt_client_subscribe( client, "/topic/qos0", 0 );
            ESP_LOGI( TAG, "sent subscribe successful, msg_id=%d", msg_id );

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI( TAG, "MQTT_EVENT_DISCONNECTED" );
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI( TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id );
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI( TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id );
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI( TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id );
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI( TAG, "MQTT_EVENT_DATA" );
            printf( "TOPIC=%.*s\r\n", event->topic_len, event->topic );
            printf( "DATA=%.*s\r\n", event->data_len, event->data );
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI( TAG, "MQTT_EVENT_ERROR" );
            break;
            
        default:
            ESP_LOGI( TAG, "Other event id:%d", event->event_id );
            break;
    }
}

static void mqtt_client_start( void )
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = BROKER_URL,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init( &mqtt_cfg );
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event( client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL );
    esp_mqtt_client_start( client );
}

/* registered event handler to get wifi connection related events/interrupts */
static void event_handler( void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data )
{
    
    if ( event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START ) 
    {
        esp_wifi_connect();
    }
    else if( event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED )
    {
        esp_wifi_connect();
        ESP_LOGI( TAG, "retry to connect to the AP" );
    
    }
    else if( event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP )
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;

        ESP_LOGI( TAG, "got ip:" IPSTR, IP2STR( &event->ip_info.ip ) );
        got_ip=true;
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    /* Initialise tcp ip stack for ip */
    ESP_ERROR_CHECK( esp_netif_init() );

    ESP_ERROR_CHECK( esp_event_loop_create_default() );
    esp_netif_create_default_wifi_sta();

    /* Initialise wifi basic configuration's for esp32 */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    esp_event_handler_instance_t instance_any_id, instance_got_ip;

    /* Register event handler to get all events/interrupts reslated to esp32 */
    ESP_ERROR_CHECK( esp_event_handler_instance_register( WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id ) );

    /* Register event handler to IP related events/interrupts */
    ESP_ERROR_CHECK( esp_event_handler_instance_register( IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip ) );

    /* Set ssid & password to connect with your access point */
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    /* set station mode */
    ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ) );
    /* set your wifi credentials */
    ESP_ERROR_CHECK( esp_wifi_set_config( WIFI_IF_STA, &wifi_config ) );
    /* start your wifi stack */
    ESP_ERROR_CHECK( esp_wifi_start() );

    ESP_LOGI( TAG, "wifi_init_sta finished." );
}

void app_main( void )
{
    /* Initialize NVS to store wifi credentials */
    esp_err_t ret = nvs_flash_init();
    if ( ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND )
    {
      ESP_ERROR_CHECK( nvs_flash_erase() );
      ret = nvs_flash_init();
    }

    /* check return value for error handling */
    ESP_ERROR_CHECK( ret );

    wifi_init_sta();
    while ( !got_ip )
    {
        vTaskDelay( 10 );
    }

    mqtt_client_start();
}
