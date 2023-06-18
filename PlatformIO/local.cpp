#include <M5StickCPlus.h>
#include <WiFi.h>
#include <time.h>

int data_num = 20000;
uint16_t sdata[20000];
// char data_to_send[200000] = {0}; // 各データ最大4桁+データ間のスラッシュ = 160000+40000 = 200000
char data_chunk[20000] = {0};
int chunk_size = 20000;

char ssid[] = "";
char pass[] = "";
const char *host = "";

const int port = 10000;

WiFiClient client;

void connectWiFi();
void connectTCP();
int convert_to_array(int, int);
void reset_array();

void setup()
{
    // GPIO Setup
    M5.begin();
    pinMode(36, INPUT); // WiFiを使うときは36ピンしかADCできない
    gpio_pulldown_dis(GPIO_NUM_25);
    gpio_pullup_dis(GPIO_NUM_25);
    analogSetAttenuation(ADC_6db);

    // Display Setup
    M5.Lcd.setRotation(3); // 画面を横向きにする
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE);

    // WiFi Setup
    connectWiFi();

    // TCP Setup
    // connectTCP();

    delay(500);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(5, 30);
    M5.Lcd.println("MIC TEST");
}

void loop()
{
    // if (!client.connected())
    // {
    //   M5.Lcd.fillScreen(BLACK);
    //   M5.Lcd.setCursor(0, 0);
    //   M5.Lcd.println("connection lost");
    //   connectTCP();
    //   delay(500);
    // }

    M5.update();

    if (M5.BtnA.wasReleased())
    {

        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(5, 30);
        M5.Lcd.printf("RECORDING");

        // Serial.print(":");
        // Serial.println(millis());
        for (int i = 0; i < data_num; i++)
        {
            sdata[i] = analogRead(36);
            // Serial.println(sdata[i]);
            delayMicroseconds(180);
        }
        // Serial.print(":");
        // Serial.println(millis());

        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(5, 30);
        M5.Lcd.println("RECORDED PLZ WAIT");

        connectTCP();

        int count = 0;
        for (int i = 0; i < data_num; i++)
        {
            int res = sdata[i];
            int mv = (int)(3300 * float(res) / log(5) / 4095.0);

            // Serial.printf("count: %d\n", count);
            // Serial.printf("chunk: %s\n", data_chunk);
            // client.printf("%d/", mv);
            count += convert_to_array(mv, count);
            // Serial.println(strlen(data_chunk));
            if ((i + 1) % 4000 == 0)
            {

                client.print(data_chunk);
                reset_array();
                count = 0;
            }
        }
        client.print("....");
        client.stop();
        M5.Lcd.println("message sent");
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(5, 30);
        M5.Lcd.printf("MIC TEST");
    }

    if (M5.BtnB.wasReleased())
    {
    }
    delay(1);
}

void connectWiFi()
{
    M5.Lcd.println("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    time_t prevtime = time(NULL);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        M5.Lcd.print(".");
        if (time(NULL) - prevtime > 60)
        {
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.println("WiFi connection failure");
            while (1)
            {
                delay(100);
            } // 処理を終了
        }
    }
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("WiFi connected");
}

void connectTCP()
{
    M5.Lcd.println("connecting to server");

    time_t prevtime = time(NULL);
    while (!client.connect(host, port))
    {
        delay(500);
        M5.Lcd.print(".");
        if (time(NULL) - prevtime > 60)
        {
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.println("TCP connection failure");
            while (1)
            {
                delay(100);
            }
        }
    }
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("TCP connected");

    client.setTimeout(100);
}

int convert_to_array(int mv, int index)
{
    char temp[5] = {0};
    itoa(mv, temp, 10);
    if (mv < 1000)
    {
        data_chunk[index] = '0';
        for (int i = 0; i < 3; i++)
        {
            data_chunk[index + i + 1] = temp[i];
        }
        // data_chunk[index + 3] = '/';
        return 4;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            data_chunk[index + i] = temp[i];
        }
        // data_chunk[index + 4] = '/';
        // return 5;
        return 4;
    }
}

void reset_array()
{
    for (int i = 0; i < chunk_size; i++)
    {
        data_chunk[i] = 0;
    }
}
