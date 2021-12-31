#include "GU140X32F_7703A.h"


//#define DEBUG_TXT(x)  Serial.print(x)
#define DEBUG_TXT(x)  0
//#define DEBUG_NUM(x)  Serial.print(x, HEX)
#define DEBUG_NUM(x)  0
//#define DEBUG_OUT(x)  Serial.println(x)
#define DEBUG_OUT(x) 0
//#define DEBUG_SIG(x)  Serial.println(x)
#define DEBUG_SIG(x) 0

#define OUT sin
#define BUSY sbusy
#define SCK sck
#define RESET reset


#define DIRECTION(X,D)  if (D) {pinMode(X, OUTPUT);} else {pinMode(X, INPUT);}
#define RAISE(X)  digitalWrite(X, HIGH)
#define LOWER(X)  digitalWrite(X, LOW)
#define CHECK(X)  digitalRead(X)


GU140X32F_7703A::GU140X32F_7703A(uint8_t sin, uint8_t sbusy, uint8_t sck, uint8_t reset) : Adafruit_GFX(140, 32)
{
    this->sin = sin;
    this->sbusy = sbusy;
    this->sck = sck;
    this->reset = reset;
    this->liveMode = 1;
    pinMode(13, OUTPUT);
    this->minX=1000;
    this->maxX=-1;
}

void GU140X32F_7703A::init_pins() {
  LOWER(OUT);
  RAISE(SCK);
  RAISE(RESET);
  DIRECTION(RESET,1);
  DIRECTION(OUT,1);
  DIRECTION(SCK,1);
  DIRECTION(BUSY,0);
}

void GU140X32F_7703A::vfdWrite16(uint16_t data) {
  DEBUG_TXT("write: ");
  DEBUG_NUM(data);
  DEBUG_OUT("");
  while (CHECK(BUSY));
  for (uint16_t i=1; i; i<<=1) {
     LOWER(SCK);
     digitalWrite(OUT, (data & i));
     _delay_us(1);
     RAISE(SCK);
     _delay_us(1);
   }
   _delay_us(17);
}

void GU140X32F_7703A::vfdWrite8(uint8_t data) {
  DEBUG_TXT("write: ");
  DEBUG_NUM(data);
  DEBUG_OUT("");
  while (CHECK(BUSY));
  for (uint8_t i=1; i; i<<=1) {
     LOWER(SCK);
     digitalWrite(OUT, (data & i));
     _delay_us(1);
     RAISE(SCK);
     _delay_us(1);
   }
   _delay_us(17);
}

void GU140X32F_7703A::hardReset() {
  init_pins();
  LOWER(RESET);
  delay(1);
  RAISE(RESET);
  delay(100);
  while (CHECK(BUSY));
}

void GU140X32F_7703A::setVFDCursor(int16_t x, int16_t y) {
  vfdWrite8(0x1F);
  vfdWrite8(0x24);
  vfdWrite16(x);
  vfdWrite16(y);
}

void GU140X32F_7703A::writeByte(int8_t b) {
  vfdWrite8(0x1F);
  vfdWrite8(0x28);
  vfdWrite8(0x66);
  vfdWrite8(0x11);
  vfdWrite8(0x01);
  vfdWrite8(0x00);
  vfdWrite8(0x01);
  vfdWrite8(0x00);
  vfdWrite8(0x01);
  vfdWrite8(b);
}

void GU140X32F_7703A::vfdWriteImageData(uint8_t * data, uint16_t pixelWidth, uint16_t byteHeight) {
  uint16_t numBytes = pixelWidth * byteHeight;
  uint16_t idx;
  vfdWrite8(0x1F);
  vfdWrite8(0x28);
  vfdWrite8(0x66);
  vfdWrite8(0x11);
  vfdWrite16(pixelWidth);
  vfdWrite16(byteHeight);
  vfdWrite8(0x01);
  for (idx = 0; idx < numBytes; idx++) {
    vfdWrite8(data[idx]);
  }
}


void GU140X32F_7703A::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 140 && y < 32){
        int16_t byteY = y >> 3;
        int8_t maskY = 0x80 >> (y & 0x07);
        int8_t b = buf[x * 4 + byteY];
        if (color > 0)
            b = b | maskY;
        else
            b = b & (~maskY);
        buf[x * 4 + byteY] = b;
        if (liveMode) {
          setVFDCursor(x, byteY);
          writeByte(b);
        }
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
    }
}

void GU140X32F_7703A::display(int16_t extend){
  if (maxX >=0){
    maxX += extend + 1;
    minX -= extend;
    minX = minX >=0 ? minX : 0;
    maxX = maxX <=140 ? maxX : 140;
    int16_t width = maxX - minX;
    int16_t startIdx  = minX * 4;
    setVFDCursor(minX,0);
    vfdWriteImageData(&buf[startIdx],width,4);
  }
  minX=1000;
  maxX=-1;
  
}

void GU140X32F_7703A::clear(){
  minX=1000;
  maxX=-1;
  for (int idx = 0; idx < sizeof(buf); idx ++){
    if (buf[idx] != 0){
      int16_t x = idx / 4;
      if (x < minX) minX = x;
      if (x > maxX) maxX = x;
    }
  }
  memset(buf, 0, sizeof(buf));
}

void GU140X32F_7703A::displayOn(bool on){
  vfdWrite8(0x1f);
  vfdWrite8(0x28);
  vfdWrite8('a');
  vfdWrite8(0x40);
  vfdWrite8(on ? 1 : 0);
}

void GU140X32F_7703A::brightness(uint8_t level){

  if (level == 0){
    displayOn(false);
  } else if (level <= 100){
    displayOn(true);
    vfdWrite8(0x1F);
    vfdWrite8('X');
    vfdWrite8((level*10 + 120)/125);
  }
}
