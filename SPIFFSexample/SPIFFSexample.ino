#include "FS.h"
#include "SPIFFS.h"

#include <M5StickCPlus.h>

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    M5.Lcd.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        M5.Lcd.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        M5.Lcd.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            M5.Lcd.print("  DIR : ");
            M5.Lcd.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            M5.Lcd.print("  FILE: ");
            M5.Lcd.print(file.name());
            M5.Lcd.print("\tSIZE: ");
            M5.Lcd.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char *path)
{
    M5.Lcd.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        M5.Lcd.println("- failed to open file for reading");
        return;
    }

    M5.Lcd.println("- read from file:");
    while (file.available())
    {
        M5.Lcd.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    M5.Lcd.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        M5.Lcd.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        M5.Lcd.println("- file written");
    }
    else
    {
        M5.Lcd.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    M5.Lcd.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        M5.Lcd.println("- failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        M5.Lcd.println("- message appended");
    }
    else
    {
        M5.Lcd.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    M5.Lcd.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        M5.Lcd.println("- file renamed");
    }
    else
    {
        M5.Lcd.println("- rename failed");
    }
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

void testFileIO(fs::FS &fs, const char *path)
{
    M5.Lcd.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        M5.Lcd.println("- failed to open file for writing");
        return;
    }

    size_t i;
    M5.Lcd.print("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
            M5.Lcd.print(".");
        }
        file.write(buf, 512);
    }
    M5.Lcd.println("");
    uint32_t end = millis() - start;
    M5.Lcd.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        M5.Lcd.print("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
                M5.Lcd.print(".");
            }
            len -= toRead;
        }
        M5.Lcd.println("");
        end = millis() - start;
        M5.Lcd.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    }
    else
    {
        M5.Lcd.println("- failed to open file for reading");
    }
}

void setup()
{
    M5.Lcd.begin();
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        M5.Lcd.println("SPIFFS Mount Failed");
        return;
    }

    listDir(SPIFFS, "/", 0);
    writeFile(SPIFFS, "/hello.txt", "Hello ");
    appendFile(SPIFFS, "/hello.txt", "World!\r\n");
    readFile(SPIFFS, "/hello.txt");
    renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    readFile(SPIFFS, "/foo.txt");
    deleteFile(SPIFFS, "/foo.txt");
    testFileIO(SPIFFS, "/test.txt");
    deleteFile(SPIFFS, "/test.txt");
    M5.Lcd.println("Test complete");
}

void loop()
{
}