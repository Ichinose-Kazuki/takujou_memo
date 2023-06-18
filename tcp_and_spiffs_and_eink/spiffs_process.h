#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include "data_difinitions.h"

#define FORMAT_SPIFFS_IF_FAILED true

void spiffs_begin();
void readFile(fs::FS &fs, const char *path, unsigned char *image);
void writeFile(fs::FS &fs, const char *path, const unsigned char *message);
void read_img_spiffs();
void write_img_spiffs();
