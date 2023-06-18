#include "connect_server.h"

WiFiClient client;

uint16_t sdata[DATA_NUM];
// char data_to_send[200000] = {0}; // 各データ最大4桁+データ間のスラッシュ = 160000+40000 = 200000
char data_chunk[CHUNK_SIZE];
extern unsigned char image[IMG_SIZE];
extern int image_index;

void connectWiFi()
{
    Serial.println("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    time_t prevtime = time(NULL);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (time(NULL) - prevtime > 60)
        {
            Serial.println("WiFi connection failure");
            while (1)
            {
                delay(100);
            } // 処理を終了
        }
    }
    Serial.println("WiFi connected");
}

void connectTCP()
{
    Serial.println("connecting to server");

    time_t prevtime = time(NULL);
    while (!client.connect(SERVER_HOST, SERVER_PORT))
    {
        delay(500);
        Serial.print(".");
        if (time(NULL) - prevtime > 60)
        {
            Serial.println("TCP connection failure");
            while (1)
            {
                delay(100);
            }
        }
    }
    Serial.println("TCP connected");

    client.setTimeout(100);
}

void disconnectTCP()
{
    client.stop();
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
        return 4;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            data_chunk[index + i] = temp[i];
        }
        return 4;
    }
}

void reset_array()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        data_chunk[i] = 0;
    }
}

void rec_and_send_mic_data()
{

    Serial.println("RECORDING");

    digitalWrite(REC_LED_PIN, HIGH);
    for (int i = 0; i < DATA_NUM; i++)
    {
        sdata[i] = analogRead(MIC_PIN);
        delayMicroseconds(180);
    }
    digitalWrite(REC_LED_PIN, LOW);

    Serial.println("RECORDED PLZ WAIT");

    if (digitalRead(LANG_SW_PIN))
    {
        client.print('j');
    }
    else
    {
        client.print('e');
    }

    int count = 0;
    for (int i = 0; i < DATA_NUM; i++)
    {
        int res = sdata[i];
        int mv = (int)(3300 * float(res) / log(5) / 4095.0);

        count += convert_to_array(mv, count);
        if ((i + 1) % 4000 == 0)
        {

            client.print(data_chunk);
            reset_array();
            count = 0;
        }
    }
    client.print("...."); // データの終わりの印
}

void receive_img_data()
{
    // 何か送られてくるまで待つ
    int counter_tmp = 0;
    while (client.read() == -1)
    {
        delay(100);
        counter_tmp++;
        if (counter_tmp > 100)
        {
            Serial.println("no data received");
            break;
        }
    }
    int success = client.read(image, IMG_SIZE);

    // for (int i = 0; i < IMG_SIZE; i++)
    // {
    //     if (image[i] != 255)
    //     {
    //         Serial.println(image[i]);
    //     }
    // }
    Serial.println("message sent and received");
    Serial.println(success);
}
