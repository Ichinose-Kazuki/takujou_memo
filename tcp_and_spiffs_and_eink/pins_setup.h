#pragma once

#include <Arduino.h>

#define EPD1_SCK_PIN 13
#define EPD1_MOSI_PIN 14
#define EPD1_CS_PIN 15
#define EPD1_RST_PIN 26
#define EPD1_DC_PIN 27
#define EPD1_BUSY_PIN 25

#define EPD2_SCK_PIN 18
#define EPD2_MOSI_PIN 19
#define EPD2_CS_PIN 5
#define EPD2_RST_PIN 3
#define EPD2_DC_PIN 1
#define EPD2_BUSY_PIN 4

#define MIC_PIN 36
#define REC_SW_PIN 33
#define SELECT_SW_PIN 32

#define LANG_SW_PIN 21

#define REC_LED_PIN 22

void gpio_begin();
