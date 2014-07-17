/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 * Home Appliance Control Application
 *
 * This application demonstrates how a simple web page can be used to send
 * information to a UART when a button on the webpage is clicked. The application
 * mimics a very basic user interface to control a home appliance such as a washing
 * machine or dryer.
 *
 * Features demonstrated
 *  - WICED Configuration Mode
 *  - Wi-Fi client mode
 *  - HTTP web server with dynamic content
 *  - Powersave
 *
 * Application Instructions
 *   1. Modify the CLIENT_AP_SSID/CLIENT_AP_PASSPHRASE Wi-Fi credentials
 *      in the wifi_config_dct.h header file to match your Wi-Fi access point
 *   2. Connect a PC terminal to the serial port of the WICED Eval board,
 *      then build and download the application as described in the WICED
 *      Quick Start Guide
 *   3. After the download completes, the terminal displays WICED startup
 *      information and starts WICED Configuration Mode.
 *
 *   If you are unfamiliar with how WICED Configuration Mode works,
 *   please step through the config_mode snippet application in the
 *   Apps/snip directory before continuing.
 *
 *   Once the device has been configured, it connects as a client to the Wi-Fi AP
 *   selected during configuration mode. The app starts a web server that displays a
 *   web page with clickable buttons. A Wi-Fi client (eg. a computer)
 *   connected to the same home Wi-Fi network as the WICED module can then connect
 *   to the appliance web server using a web browser.
 *
 *   To view the Appliance web page in a web browser:
 *     - Open a web browser and go to http://192.168.1.99
 *       (replace this IP address with the IP of the WICED device; the IP
 *        address of the WICED device is available on the terminal after
 *        WICED Config Mode completes)
 *     - If your computer has an mDNS (or Bonjour) network service browser,
 *       the WICED device can be found by using the browser to search for
 *       and connect to the service called "Appliance Webserver".
 *     - When a button on the webpage is clicked, button click information
 *       prints to the UART and is shown on the terminal
 *
 * Powersave Demonstration
 *   To demonstrate usage of the MCU & Wi-Fi powersave API in an application with
 *   an active network connection, a timed event is configured to intermittently
 *   enable, and then disable, MCU powersave mode. Using a WICED evaluation board,
 *   the power (current) profile can be measured on an oscilloscope by connecting
 *   to the testpoints labelled CURRENT & GND (J1). If you are unfamiliar with the
 *   WICED evaluation board or the WICED powersave API, refer to the WICED
 *   Evaluation Board User Guide and WICED Powersave Application Note in the
 *   <WICED-SDK>/Doc directory
 *
 * Notes.
 *   1. This application only works with Access Points configured for Open/WPA/WPA2
 *      security. It is not configured to work with APs that use WEP security!
 *
 */

#include "wiced.h"
#include "resources.h"
#include "http_server.h"
#include "gedday.h"



/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

static int            process_button_handler( const char* url, wiced_tcp_stream_t* stream, void* arg );
static int            process_fb_update     ( const char* url, wiced_tcp_stream_t* stream, void* arg );
static wiced_result_t powersave_toggle( void* arg );

/******************************************************
 *               Variables Definitions
 ******************************************************/

START_OF_HTTP_PAGE_DATABASE(appliance_web_pages)
    ROOT_HTTP_PAGE_REDIRECT("/apps/wicedhack/main.html"),
    { "/apps/wicedhack/main.html",    "text/html",                WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_apps_DIR_wicedhack_DIR_main_html, sizeof(resource_apps_DIR_wicedhack_DIR_main_html)-1}, },
        { "/fb_report.html",               "text/html",                WICED_DYNAMIC_URL_CONTENT, .url_content.dynamic_data = {process_fb_update, 0 }, },
        //{ "/temp_up",                        "text/html",                WICED_DYNAMIC_URL_CONTENT, .url_content.dynamic_data = {process_temperature_up, 0 }, },
        //{ "/temp_down",                      "text/html",                WICED_DYNAMIC_URL_CONTENT, .url_content.dynamic_data = {process_temperature_down, 0 }, },
        { "/images/favicon.ico",             "image/vnd.microsoft.icon", WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_images_DIR_favicon_ico, sizeof(resource_images_DIR_favicon_ico)}, },
        { "/scripts/general_ajax_script.js", "application/javascript",   WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_scripts_DIR_general_ajax_script_js, sizeof(resource_scripts_DIR_general_ajax_script_js)-1 }, },
        { "/images/brcmlogo.png",            "image/png",                WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_images_DIR_brcmlogo_png, sizeof(resource_images_DIR_brcmlogo_png)}, },
        { "/images/brcmlogo_line.png",       "image/png",                WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_images_DIR_brcmlogo_line_png, sizeof(resource_images_DIR_brcmlogo_line_png)}, },
        { "/styles/buttons.css",             "text/css",                 WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_styles_DIR_buttons_css, sizeof(resource_styles_DIR_buttons_css)}, },
        { "/",                         "text/html",                WICED_STATIC_URL_CONTENT,  .url_content.static_data  = {resource_apps_DIR_wicedhack_DIR_top_web_page_top_html, sizeof(resource_apps_DIR_wicedhack_DIR_top_web_page_top_html)-1}, },
        { "/button_handler",           "text/html",                WICED_DYNAMIC_URL_CONTENT, .url_content.dynamic_data = {process_button_handler, 0}, },
END_OF_HTTP_PAGE_DATABASE();


static wiced_http_server_t http_server;
static wiced_timed_event_t powersave_toggle_event;


/******************************************************
 *               Function Definitions
 ******************************************************/

void application_start(void)
{
    /* Initialise the device and WICED framework */
    wiced_init( );

    /* Configure the device */
    wiced_configure_device( NULL );

    /* Bring up the network interface */
    wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );

    /* Enable MCU powersave. MCU powersave API can be globally DISABLED in <WICED-SDK>/include/wiced_defaults.h */
    /* WARNING: Please read the WICED Powersave API documentation before enabling MCU powersave */
    //wiced_platform_mcu_enable_powersave();

    /* Enable 802.11 powersave mode */
    wiced_wifi_enable_powersave_with_throughput( 40 );

    /* Register an event that toggles powersave every 2 seconds to
     * demonstrate power consumption with & without powersave enabled
     */
    wiced_rtos_register_timed_event( &powersave_toggle_event, WICED_HARDWARE_IO_WORKER_THREAD, &powersave_toggle, 2000, 0 );

    /* Start web-server */
    wiced_http_server_start( &http_server, 80, appliance_web_pages, WICED_STA_INTERFACE );

    /* Advertise webpage services using Gedday */
    gedday_init( WICED_STA_INTERFACE, "wiced-appliance-app" );
    gedday_add_service( "Appliance Webserver", "_http._tcp.local", 80, 300, "" );
}

static int process_fb_update ( const char* url, wiced_tcp_stream_t* stream, void* arg ){
    printf( "\r\nThis is where the FB update will appear: %d\r\n\r\n", 999 );
    return 0;
}
static int process_button_handler( const char* url, wiced_tcp_stream_t* stream, void* arg )
{
    int params_len = strlen(url);
    char * found_loc = NULL;
    char  end_found = 0;

    /* Process the GET parameter list to determine if buttons have been pressed */

    /* Cycle through parameter list string until end or newline */
    while ( end_found == 0 )
    {
        /* Check if parameter is "led" */
        if ( 0 == strncmp( url, "btname", 6 ) )
        {
            found_loc = (char*)&url[7];
        }

        /* Scan ahead to the next parameter or the end of the parameter list */
        while ( ( *url != '&' ) && ( *url != '\n' ) && ( params_len > 0 ) )
        {
            url++;
            params_len--;
        }

        if  ( *url != '&' )
        {
            end_found = 1;
        }


        if ( found_loc != NULL )
         {
             char* tmp = (char*)url;
             *tmp = '\x00';
             printf( "\r\nDetected button press: %s\r\n\r\n", found_loc );
         }


        if ( end_found == 0 )
        {
            /* Skip over the "&" which joins parameters if found */
            url++;
        }
    }

    /* Send the html page back to the client */
    wiced_tcp_stream_write( stream, resource_apps_DIR_wicedhack_DIR_main_html, sizeof(resource_apps_DIR_wicedhack_DIR_main_html) );

    return 0;
}


wiced_result_t powersave_toggle( void* arg )
{
    static wiced_bool_t powersave_enabled = WICED_TRUE;
    if( powersave_enabled )
    {
        wiced_platform_mcu_disable_powersave( );
        powersave_enabled = WICED_FALSE;
    }
    else
    {
        wiced_platform_mcu_enable_powersave( );
        powersave_enabled = WICED_TRUE;
    }

    return WICED_SUCCESS;
}
