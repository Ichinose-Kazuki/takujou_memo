#include <M5StickCPlus.h>
#include <Preferences.h>
Preferences prefs;

#define IMAGE_LENGTH 4796

char image[IMAGE_LENGTH] = {0};
char buffer[IMAGE_LENGTH] = {0};

void setup()
{
    M5.begin();
    M5.Lcd.println("start saving");

    prefs.begin("images", false);

    for (int i = 0; i < 10; i++)
    {
        image[0] = i;
        char namebuf[10];
        String name = "image";
        name += i;
        name.toCharArray(namebuf, 10);
        M5.Lcd.println(namebuf);

        prefs.putBytes(namebuf, image, IMAGE_LENGTH);
    }

    prefs.end();
    M5.Lcd.println("end saving");

    M5.Lcd.println("get stored image");

    prefs.begin("images");

    for (int i = 0; i < 10; i++)
    {
        char namebuf[10];
        String name = "image";
        name += i;
        name.toCharArray(namebuf, 10);

        prefs.getBytes(namebuf, buffer, IMAGE_LENGTH);
        M5.Lcd.printf("%d\n", buffer[0]);
    }

    prefs.end();

    M5.Lcd.println("end");
}

void loop() {}