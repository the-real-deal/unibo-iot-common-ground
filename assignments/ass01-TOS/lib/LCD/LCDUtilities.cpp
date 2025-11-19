#include <LCDUtilities.h>
#include <assert.h>
#include <Arduino.h>

void displayMessage(LiquidCrystal_I2C *display, const String& message)
{   
    display->clear();
    int offsetX = 0, offsetY = 0;
    const int messageLength = message.length();
    for (int i = 0; i < messageLength; i++) 
    {
        assert(offsetY <= MAX_LINES_IDX);
        if (message[i] == '\n')
        {
            offsetX = 0;
            offsetY++;
            continue;
        }
        display->setCursor(offsetX++, offsetY);
        display->print(message[i]);
    }
}

void initLCD(LiquidCrystal_I2C *display)
{
    display->init();
    display->backlight();
}
