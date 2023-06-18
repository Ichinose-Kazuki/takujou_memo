#include "FS.h"
#include "SPIFFS.h"

#include <M5StickCPlus.h>

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

#define IMAGE_LENGTH 4736

unsigned char image[IMAGE_LENGTH];

unsigned char buffer[IMAGE_LENGTH];

void readFile(fs::FS &fs, const char *path)
{
    M5.Lcd.printf("file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        M5.Lcd.println("- failed to open file for reading");
        return;
    }

    M5.Lcd.println(file.size());

    // M5.Lcd.println("- read from file:");
    while (file.available())
    {
        M5.Lcd.write(file.read());
    }
    M5.Lcd.println();
    file.close();
}

void writeFile(fs::FS &fs, const char *path, const unsigned char *message)
{
    // M5.Lcd.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        M5.Lcd.println("- failed to open file for writing");
        return;
    }
    int written = 0;
    if (written = file.write(message, IMAGE_LENGTH))
    {
        M5.Lcd.println(written);
    }
    else
    {
        M5.Lcd.println("- write failed");
    }
    file.close();
}

void deleteFile(fs::FS &fs, const char *path)
{
    M5.Lcd.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path))
    {
        M5.Lcd.println("- file deleted");
    }
    else
    {
        M5.Lcd.println("- delete failed");
    }
}

void setup()
{
    M5.begin();
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        M5.Lcd.println("SPIFFS Mount Failed");
        return;
    }

    M5.Lcd.println("writing");
    for (int i = 0; i < 10; i++)
    {
        image[0] = 'i';
        image[IMAGE_LENGTH - 1] = 'j';
        char namebuf[10];
        String name = "/image";
        name += (i + 1);
        name.toCharArray(namebuf, 10);
        // M5.Lcd.println(namebuf);
        writeFile(SPIFFS, namebuf, image);
    }

    M5.Lcd.println("reading");
    for (int i = 0; i < 10; i++)
    {
        char namebuf[10];
        String name = "/image";
        name += (i + 1);
        name.toCharArray(namebuf, 10);
        // M5.Lcd.println(namebuf);
        readFile(SPIFFS, namebuf);
        // M5.Lcd.printf("%d\n", buffer[0]);
    }

    M5.Lcd.println("end");
}

void loop()
{
}