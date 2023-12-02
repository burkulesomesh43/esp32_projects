/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/tcp_client/main/main.c

  @brief
    tcp client source code.file

  @details 

  @note

  @author
    Somesh Burkule

  @date
    01/12/2023
*/
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"


#define WIFI_SSID      "edu.wireless"
#define WIFI_PASS      "IoT@1234"

#define HOST_IP_ADDR "192.168.1.5"
#define PORT 8888

/* Macro used to print logs on monitor */
#define TAG  "TCP_CLIENT"

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

bool got_ip;

short SocketCreate(void)
{
    short hSocket;
    printf("Create the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}
int BindCreatedSocket(int hSocket)
{
    int iRetval=-1;
    int ClientPort = 90190;
    struct sockaddr_in  remote= {0};
    /* Internet address family */
    remote.sin_family = AF_INET;
    /* Any incoming interface */
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(ClientPort); /* Local port */
    iRetval = bind(hSocket,(struct sockaddr *)&remote,sizeof(remote));
    return iRetval;
}

static void tcp_server_task(void *pvParameters)
{
    int socket_desc, sock, clientLen, read_size;
    struct sockaddr_in server, client;
    char client_message[200]= {0};
    char message[100] = {0};
    const char *pMessage = "welcome to eduwireless";

    //Create socket
    socket_desc = SocketCreate();
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");

    //Bind
    if( BindCreatedSocket(socket_desc) < 0)
    {
        //print the error message
        perror("bind failed.");
        return 1;
    }

    printf("bind done\n");
    //Listen
    listen(socket_desc, 3);
    //Accept and incoming connection
    while(1)
    {
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);
        //accept connection from an incoming client
        sock = accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&clientLen);
        if (sock < 0)
        {
            perror("accept failed");
            return 1;
        }

        printf("Connection accepted\n");
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        //Receive a reply from the client
        if( recv(sock, client_message, 200, 0) < 0)
        {
            printf("recv failed");
            break;
        }

        printf("Client reply : %s\n",client_message);
        if(strcmp(pMessage,client_message)==0)
        {
            strcpy(message,"Hi there !");
        }
        else
        {
            strcpy(message,"Invalid Message !");
        }
        
        // Send some data
        if( send(sock, message, strlen(message), 0) < 0)
        {
            printf("Send failed");
            return 1;
        }
        close(sock);
        sleep(1);
    }
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
    ESP_ERROR_CHECK( esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id ) );

    /* Register event handler to IP related events/interrupts */
    ESP_ERROR_CHECK(esp_event_handler_instance_register( IP_EVENT,
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
    if ( ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND ) {
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

    xTaskCreate( tcp_server_task, "tcp_server", 4096, NULL, 5, NULL );
}
