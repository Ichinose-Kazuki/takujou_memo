#include <M5StickCPlus.h>



int data_num=20000;
uint16_t sdata[20000];

void setup() {
  M5.begin();
  pinMode(36, INPUT);
  gpio_pulldown_dis(GPIO_NUM_25);
  gpio_pullup_dis(GPIO_NUM_25);
  analogSetAttenuation(ADC_6db);


  
  M5.Lcd.setRotation(3); // 画面を横向きにする
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(5, 30);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.printf("MIC TEST");

}



void loop() {
  M5.update();
  
  

  
  if (M5.BtnA.wasReleased()) {
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(5, 30);
    M5.Lcd.printf("RECORDING");
    
    Serial.print(":");
    Serial.println(millis());
    for(int i=0;i<data_num;i++){
      sdata[i] = analogRead(36);
      delayMicroseconds(200);
    }
    Serial.print(":");
    Serial.println(millis());
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(5, 30);
    M5.Lcd.printf("RECORDED PLZ WAIT");
    
    for(int i=0;i<data_num;i++){
      int res = sdata[i];
      int mv = (int)(3300 * float(res) / log(5) / 4095.0);
      
      Serial.println(mv);
    }
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(5, 30);
    M5.Lcd.printf("MIC TEST");
    
  }

  if(M5.BtnB.wasReleased()) {
    
  }
  delay(1);
}
