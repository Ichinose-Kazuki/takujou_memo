#include "spiffs_process.h"

extern unsigned char image[IMG_SIZE];
extern int image_index;

void spiffs_begin(){
  // SPIFFS setup
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
      Serial.println("SPIFFS Mount Failed");
      return;
  }
}

void readFile(fs::FS &fs, const char *path, unsigned char *image)
{
    
    
    Serial.printf("file: %s\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.printf("file size: %d\n", file.size());

    // ファイルから読み出す
    for (int i = 0; i < IMG_SIZE; i++)
    {
        image[i] = file.read();
    }

    file.close();
}

void writeFile(fs::FS &fs, const char *path, const unsigned char *message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    int written = 0;
    if (written = file.write(message, IMG_SIZE))
    {
        Serial.printf("written: %d\n", written);
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

void read_img_spiffs(){
  // SPIFFSから読み出す
  Serial.printf("switching to image%d\n", image_index + 1);
  char namebuf[10];
  String file_name = "/image";
  file_name += (image_index + 1);
  file_name.toCharArray(namebuf, 10);
  readFile(SPIFFS, namebuf, image);
}

void write_img_spiffs(){
  char namebuf[10];
  String file_name = "/image";
  file_name += image_index;
  file_name.toCharArray(namebuf, 10);
  writeFile(SPIFFS, namebuf, image);
}
