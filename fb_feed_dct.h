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
 */

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define MAX_FEED_ID_LEN    (10) /* Feed ID length ranges from 5 to 10 digits */
#define API_KEY_LEN        (48) /* API key length                            */
#define MAX_CHANNEL_ID_LEN (50) /* Maximum Channel ID length                 */

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    uint8_t  facebook_details_valid;
    char     facebook_feed_id[MAX_FEED_ID_LEN + 1];       // With terminating null
    char     facebook_api_key[API_KEY_LEN + 1];           // With terminating null
    char     facebook_channel_id[MAX_CHANNEL_ID_LEN + 1]; // With terminating null
    uint32_t sample_interval;
} user_dct_data_t;

/******************************************************
 *               Function Declarations
 ******************************************************/

/******************************************************
 *               Variables Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
