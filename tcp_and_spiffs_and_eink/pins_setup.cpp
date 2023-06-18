#include "pins_setup.h"

void gpio_begin()
{
  pinMode(MIC_PIN, INPUT);              // WiFiを使うときは36ピンしかADCできない
  pinMode(REC_SW_PIN, INPUT_PULLUP);    // 録音ボタン用
  pinMode(SELECT_SW_PIN, INPUT_PULLUP); // 切り替えボタン用
  pinMode(REC_LED_PIN, OUTPUT);
  pinMode(LANG_SW_PIN, INPUT_PULLUP);
  digitalWrite(REC_LED_PIN, LOW);
  analogSetAttenuation(ADC_6db);
}
