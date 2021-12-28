#ifndef __GU140X32F_7703A__
#define __GU140X32F_7703A__
#include "Arduino.h"
#include "Adafruit_GFX.h"

class GU140X32F_7703A: public Adafruit_GFX {
public:
    GU140X32F_7703A(uint8_t sin, uint8_t sbusy, uint8_t sck, uint8_t reset);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void display(int16_t extend);
    void displayOn(bool on);
    void brightness(uint8_t level);
    void clear();

private:
    uint8_t sin;
    uint8_t sbusy;
    uint8_t sck;
    uint8_t reset;

    uint8_t buf[140 * 32 / 8];

    void setVFDCursor(int16_t x, int16_t y);
    void writeByte(int8_t b);
    void vfdWriteImageData(uint8_t * data, uint16_t pixelWidth, uint16_t byteHeight);
    int16_t minX;
    int16_t maxX;

public:
    uint8_t liveMode;
    void init_pins();
    void vfdWrite16(uint16_t data);
    void vfdWrite8(uint8_t data);
    void hardReset();

};
#endif

