#pragma once

#include <WiFi.h>
#include <time.h>
#include "pins_setup.h"
#include "data_difinitions.h"


#define WIFI_SSID ""
#define WIFI_PASS ""
#define SERVER_HOST ""
#define SERVER_PORT 10000




void connectWiFi();
void connectTCP();
void disconnectTCP();
void rec_and_send_mic_data();
void receive_img_data();
