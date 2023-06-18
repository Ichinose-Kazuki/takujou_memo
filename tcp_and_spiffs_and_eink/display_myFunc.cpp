#include "display_myFunc.h"

extern unsigned char image[IMG_SIZE];
extern UBYTE *BlackImage;

void wakeup_printImg_sleep(int disp_no)
{
  select_display(disp_no);
  printf("e-Paper Init and Clear...\r\n");
  EPD_2IN9_V2_Init();
  EPD_2IN9_V2_Clear();
  DEV_Delay_ms(500);
  Paint_NewImage(BlackImage, EPD_2IN9_V2_WIDTH, EPD_2IN9_V2_HEIGHT, 270, WHITE);

  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_DrawBitMap(image);
  EPD_2IN9_V2_Display(BlackImage);

  printf("Goto Sleep...\r\n");
  EPD_2IN9_V2_Sleep(); // ディスプレイのスリープ
}
