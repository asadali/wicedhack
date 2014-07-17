/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#include "wiced_dct.h"
#include "temp_control_dct.h"


/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/* Put your Xively feed ID, API key, and channel ID here */
#define FACEBOOK_FEED_ID    "xxxxxxxxxx"
#define FACEBOOK_API_KEY    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define FACEBOOK_CHANNEL_ID "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

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
 *                 Global Variables
 ******************************************************/

DEFINE_APP_DCT(user_dct_data_t)
{
    .facebook_details_valid = 0,
    .facebook_feed_id       = FACEBOOK_FEED_ID,
    .facebook_api_key       = FACEBOOK_API_KEY,
    .facebook_channel_id    = FACEBOOK_CHANNEL_ID,
    .sample_interval      = 1000,
};

/******************************************************
 *               Function Declarations
 ******************************************************/
