#
# Copyright 2014, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := WICED_HACK

$(NAME)_SOURCES := wicedhack.c

$(NAME)_COMPONENTS := daemons/gedday

$(NAME)_RESOURCES  := apps/wicedhack/top_web_page_top.html \
                      scripts/general_ajax_script.js \
                      scripts/wpad.dat \
                      apps/wicedhack/main.html \
                      apps/wicedhack/data.html \
                      apps/wicedhack/index.html \
                      images/brcmlogo.png \
                      images/brcmlogo_line.png \
                      images/favicon.ico
                      
                      
GLOBAL_DEFINES :=
WIFI_CONFIG_DCT_H  := wifi_config_dct.h