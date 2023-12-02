/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/http_web_server/main/main.c

  @brief
    http server source code.file

  @details 

  @note

  @author
    Somesh Burkule

  @date
    01/12/2023
*/
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "string.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"


#define WIFI_SSID      "SSN@2ND FLOOR_2.4G"
#define WIFI_PASS      "9663862625"

/* Macro used to print logs on monitor */
#define TAG  "HTTP_SERVER"

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

bool got_ip;

const static char http_html_hdr[] =
    "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const static char http_index_hml[] ="<!DOCTYPE html>"
		"<html>"
		"<body>"
		"<form action=/IP>"
		"IP <input type=\"text\" placeholder=\"0.0.0.0\" name=\"IP\" required pattern=\"^([0-9]{1,3}\\.){3}[0-9]{1,3}$\" ><br>"
		"<input type=\"submit\" value=\"Submit\">"
		"</form>"
		"</body>"
		"</html>";

void webserver_action( struct netconn *conn )
{
  struct netbuf *inbuf;
  char *recBuffer;
  u16_t buflen;
  err_t err;

  err = netconn_recv( conn, &inbuf );

  if ( err == ERR_OK ) {

    netbuf_data( inbuf, ( void** )&recBuffer, &buflen );
    recBuffer[buflen]='\0';
	 printf( "recBuffer = %s\n", recBuffer );

    if ( strstr( recBuffer,"GET / HTTP" ) != NULL) {

      netconn_write( conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY );
	  netconn_write( conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY );
    }
    else if( strstr( recBuffer,"GET /IP" ) != NULL) {

    	netconn_write( conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY );
    	netconn_write( conn, "Thank you!!!", sizeof("Thank you!!!")-1, NETCONN_NOCOPY );
    }
  }

  netconn_close( conn );
  netbuf_delete( inbuf );
}

void webserver( void *pvParameters )
{
  struct netconn *conn, *newconn;
  err_t err;
  conn = netconn_new( NETCONN_TCP );
  netconn_bind( conn, NULL, 80 );
  netconn_listen( conn );
  while( 1 ) {

 		err = netconn_accept( conn, &newconn );
 		if ( err == ERR_OK ) {

 			webserver_action( newconn );

 			netconn_delete( newconn );
 		}

 		while (  got_ip == false ) {

 			ESP_LOGI( TAG,"Waiting For wifi connection : %d", got_ip );
 			vTaskDelay( 10 );
 		}

 		vTaskDelay( 1 );
  }

   netconn_close( conn );
   netconn_delete( conn );
   vTaskDelete( NULL );
}

/* registered event handler to get wifi connection related events/interrupts */
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
    }
    else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
        ESP_LOGI(TAG, "retry to connect to the AP");
    
    }
    else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;

        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        got_ip=true;
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    /* Initialise tcp ip stack for ip */
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    /* Initialise wifi basic configuration's for esp32 */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id, instance_got_ip;

    /* Register event handler to get all events/interrupts reslated to esp32 */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));

    /* Register event handler to IP related events/interrupts */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

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
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    /* set your wifi credentials */
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    /* start your wifi stack */
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

void app_main(void)
{
    /* Initialize NVS to store wifi credentials */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    /* check return value for error handling */
    ESP_ERROR_CHECK( ret );

    wifi_init_sta();
    while ( !got_ip )
    {
        vTaskDelay(10);
    }

    xTaskCreate( &webserver, "webserver", 2048, NULL, 5, NULL );
}
