#include <GU140X32F_7703A.h>
/**
 * Font file copied from https://github.com/robjen/GFX_fonts, as the library
 * is not yet in the Arduino Library manager.
 */
#include "Font5x5Fixed.h"

/******
 * Connect display to Arduino using these pins:
 * VFD pin  | Name  | Arduino I/O
 * ------------------------------
 *  2       | SIN   | 3
 *  4       | SBUSY | 4
 *  5       | SCK   | 5
 *  6       | RESET | 6
 * 
 */
GU140X32F_7703A vfd = GU140X32F_7703A(3, 4, 5, 6);

void setup() {
    delay(500); // wait for device to power up
    vfd.hardReset();
    vfd.liveMode = 0;
    int i;

    vfd.setCursor(100,0);
    vfd.print("Text");

    for (i=3; i< 60; i+= 6){
      vfd.drawLine(0,0,i, 14,1);
    }
    vfd.drawCircle(70,7,7,1);

    for (i=3; i< 9; i+= 2){
      vfd.drawRect(90-i,8-i,i*2, i*2, 1);
    }

    vfd.drawRect(0,16,140,15, 1);
    vfd.setFont(&Font5x5Fixed);
    vfd.setCursor(3,22);
    vfd.print("https://github.com/");
    vfd.setCursor(3,28);
    vfd.print("shufflebits/GU140X32F-7703A");
    vfd.display(0);

}

void loop() {
  

}
