/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include <stdlib.h>

#include "connect_server.h"
#include "pins_setup.h"
#include "spiffs_process.h"
#include "display_myFunc.h"
#include "data_difinitions.h"
#include "pins_setup.h"
#include "interfaces.h"

// uint16_t sdata[DATA_NUM];
//  char data_to_send[200000] = {0}; // 各データ最大4桁+データ間のスラッシュ = 160000+40000 = 200000
// char data_chunk[CHUNK_SIZE] = {0};
unsigned char image[IMG_SIZE];
int image_index = 0;

UBYTE *BlackImage;

/* Entry point ----------------------------------------------------------------*/
void setup()
{

    spiffs_begin();

    // GPIO Setup
    gpio_begin();

    Serial.begin(115200);
    Serial.println("Start");

    // WiFi Setup
    connectWiFi();

    printf("EPD_2IN9_V2_test Demo\r\n");
//    select_display(2);
    DEV_Module_Init();
//    select_display(1);
//    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
//    select_display(2);
    EPD_2IN9_V2_Init();
    EPD_2IN9_V2_Clear();
//    delay(200);
//    select_display(1);
//    EPD_2IN9_V2_Init();
//    EPD_2IN9_V2_Clear();
//    delay(200);

    // Create a new image cache
    // UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN9_V2_WIDTH % 8 == 0) ? (EPD_2IN9_V2_WIDTH / 8) : (EPD_2IN9_V2_WIDTH / 8 + 1)) * EPD_2IN9_V2_HEIGHT;
    if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        while (1)
        {
        }
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 270, WHITE);

    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in9);

//    select_display(2);
    EPD_2IN9_V2_Display(BlackImage);
//    select_display(1);
//    EPD_2IN9_V2_Display(BlackImage);
    DEV_Delay_ms(500);
    // #endif

    // printf("Clear...\r\n");
    // EPD_2IN9_V2_Init();
    // EPD_2IN9_V2_Clear();

    printf("Goto Sleep...\r\n");
//    select_display(1);
    EPD_2IN9_V2_Sleep();
//    select_display(2);
//    EPD_2IN9_V2_Sleep();
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
    if (DEV_Digital_Read(REC_SW_PIN) == 0)
    {
        connectTCP();
        rec_and_send_mic_data();
        receive_img_data();
        disconnectTCP();

        printf("e-Paper Init and Clear...\r\n");
        wakeup_printImg_sleep(2);

        write_img_spiffs();
        image_index += 1;

        // とりあえず最大３つまで保存
        if (image_index >= 3)
        {
            image_index = 0;
        }
    }

    if (DEV_Digital_Read(SELECT_SW_PIN) == 0)
    {

        read_img_spiffs();

        wakeup_printImg_sleep(2);
    }

    delay(1);
}
