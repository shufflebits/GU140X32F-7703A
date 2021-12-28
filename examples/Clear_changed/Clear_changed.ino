#include <GU140X32F_7703A.h>
#include <Fonts/FreeSansOblique18pt7b.h>

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
GU140X32F_7703A vfd = GU140X32F_7703A(3,4,5,6);

void setup() {
  delay(500);           // wait for device to power up  
  vfd.hardReset();
  vfd.liveMode = 0;
  vfd.setFont(&FreeSansOblique18pt7b);
}

int lastCount = -1;
char buffer[40];

void loop() {
  int x1,y1,w,h;
  int nextCount = lastCount + 1;
  if ((nextCount % 10) == 0){
    vfd.drawRect(0,0,139,31,1);
  }
  if (lastCount >= 0){
    sprintf(buffer,"%d", lastCount);
    String s = String(buffer);
    vfd.getTextBounds(s, 0,28,&x1,&y1, &w, &h);
    vfd.fillRect(x1,y1,w,h,0);
  }
  if ((nextCount % 10) == 1){
    vfd.drawRect(0,0,139,31,0);
  }
  
  vfd.setCursor(0,28);
  
  vfd.print(nextCount);
  vfd.display(0);
  lastCount++;
}
