/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-02-19
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "DEV_Config.h"


int EPD_SCK_PIN = EPD1_SCK_PIN;
int EPD_MOSI_PIN = EPD1_MOSI_PIN;
int EPD_CS_PIN = EPD1_CS_PIN;
int EPD_RST_PIN = EPD1_RST_PIN;
int EPD_DC_PIN = EPD1_DC_PIN;
int EPD_BUSY_PIN = EPD1_BUSY_PIN;

void GPIO_Config(void)
{
    pinMode(EPD_BUSY_PIN,  INPUT);
    pinMode(EPD_RST_PIN , OUTPUT);
    pinMode(EPD_DC_PIN  , OUTPUT);
    
    pinMode(EPD_SCK_PIN, OUTPUT);
    pinMode(EPD_MOSI_PIN, OUTPUT);
    pinMode(EPD_CS_PIN , OUTPUT);

    digitalWrite(EPD_CS_PIN , HIGH);
    digitalWrite(EPD_SCK_PIN, LOW);
}
/******************************************************************************
function:	Module Initialize, the BCM2835 library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_Module_Init(void)
{
	//gpio
	GPIO_Config();

	//serial printf
	//Serial.begin(115200);

	// spi
	// SPI.setDataMode(SPI_MODE0);
	// SPI.setBitOrder(MSBFIRST);
	// SPI.setClockDivider(SPI_CLOCK_DIV4);
	// SPI.begin();

	return 0;
}

/******************************************************************************
function:
			SPI read and write
******************************************************************************/
void DEV_SPI_WriteByte(UBYTE data)
{
    //SPI.beginTransaction(spi_settings);
    digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);

    for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0) digitalWrite(EPD_MOSI_PIN, GPIO_PIN_RESET); 
        else                    digitalWrite(EPD_MOSI_PIN, GPIO_PIN_SET);

        data <<= 1;
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
    }

    //SPI.transfer(data);
    digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
    //SPI.endTransaction();	
}

void select_display(int disp_no){
  if(disp_no==1){
    EPD_SCK_PIN = EPD1_SCK_PIN;
    EPD_MOSI_PIN = EPD1_MOSI_PIN;
    EPD_CS_PIN = EPD1_CS_PIN;
    EPD_RST_PIN = EPD1_RST_PIN;
    EPD_DC_PIN = EPD1_DC_PIN;
    EPD_BUSY_PIN = EPD1_BUSY_PIN;
  }else if(disp_no==2){
    EPD_SCK_PIN = EPD2_SCK_PIN;
    EPD_MOSI_PIN = EPD2_MOSI_PIN;
    EPD_CS_PIN = EPD2_CS_PIN;
    EPD_RST_PIN = EPD2_RST_PIN;
    EPD_DC_PIN = EPD2_DC_PIN;
    EPD_BUSY_PIN = EPD2_BUSY_PIN;
  }else{
    EPD_SCK_PIN = EPD1_SCK_PIN;
    EPD_MOSI_PIN = EPD1_MOSI_PIN;
    EPD_CS_PIN = EPD1_CS_PIN;
    EPD_RST_PIN = EPD1_RST_PIN;
    EPD_DC_PIN = EPD1_DC_PIN;
    EPD_BUSY_PIN = EPD1_BUSY_PIN;
  }
}
