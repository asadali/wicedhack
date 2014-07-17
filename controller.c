/*
 * controller.c
 *
 *  Created on: Jul 17, 2014
 *      Author: jvivek
 */

/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#include <stdio.h>
#include <string.h>
#include <FreeRTOS.h>
#include <timers.h>
#include "rtos.h"
#include "wiced.h"

#define ONE_MINUTE  60
#define NO_WAIT     0

static int be_back_minutes;
char *time;
wiced_timer_t* back_in_timer;
char str1[] = "Will be back in ";
char str3[] = " minutes";

void initall(void)
{
    initialize_1minute_timer( back_in_timer );
    /* Initialise ring buffer */
    ring_buffer_init(&rx_buffer, rx_data, RX_BUFFER_SIZE );

    /* Initialise UART. A ring buffer is used to hold received characters */
}

wiced_uart_config_t uart_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};

wiced_ring_buffer_t rx_buffer;

void convert_to_lower( char *word )
{
    int count = 0;
    while ( word[count] )
    {
        word[count] = tolower( word[count] );
        count++;
    }
    return;
}

native_timer_handler_t check_time_remaining(xTimerHandle pxTimer)
{
    if(be_back_minutes != 0)
    {
        // Display minutes remaining
    }
}

void initialize_1minute_timer( wiced_timer_t* timer )
{
    timer->handle = xTimerCreate(  (const signed char *)"", ONE_MINUTE, pdTRUE, timer, (native_timer_handler_t) check_time_remaining );
}


void check_command( char *cmd_str[], int len )
{
    char cmd;

    if ( strcmp( cmd_str[0], "turn" ) )
    {
        if ( strcmp( cmd_str[1], "on" ) )
        {
            cmd = 'a'; // Turn on some device
        }
        else if ( strcmp( cmd_str[1], "off" ) )
        {
            cmd = 'b'; // Turn off some device
        }
        else
        {
            cmd = 'x';
            return;
        }
    }
    else if ( strcmp( cmd_str[1], "back" ) && strcmp( cmd_str[2], "in" ) )
    {
        cmd = 't'; // Display back in minutes and invoke timer.
        be_back_minutes = 25;
        itoa(be_back_minutes,time,10);
        char *str2;
        str2 = (char*)malloc(strlen(str1) + strlen(time) + strlen(str3) + 1);
        strcat(str2,str1);
        strcat(str2,time);
        strcat(str2,str3);
        wiced_uart_transmit_bytes( WICED_UART_1, str2, (sizeof(str2)-1) );
        xTimerStart( back_in_timer->handle, NO_WAIT );
    }
    else if ( strcmp( cmd_str[1], "percent" ) )
    {
        cmd = 'p'; // Adjust device working percentage
    }
    else if ( strcmp( cmd_str[1], "display" ) )
    {
        cmd = 'd'; // Display status
        if ( strcmp( cmd_str[2], "all" ) )
        {
            // Display status of all devices
        }
    }
}

/*------------------------------------------------------------------------------
 *
 * Function name : parse_command(char *str)
 * Description : Function to parse the received string, determine the length of
 * the string, identify the command and execute it.
 *
 *
 */

void parse_command( char *str )
{
    char temp[8], segments[8];
    char *pch1, *pch2;
    int len = 0, count;
    pch = strtok( str, " -" );
    while ( pch1 != NULL )
    {
        len++;
        pch1 = strtok( str, NULL, " -" );
    }
    segment[0] = strtok( str, " -" );
    for ( count = 1; count < len; count++ )
    {
        segment[count] = strtok( NULL, " -" );
        convert_to_lower( segment[count] );
    }
    check_command( segment, len );
}
