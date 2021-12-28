#include <GU140X32F_7703A.h>

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
}

int interval = 0;

int brightness = 0;

void loop() {
    int newinterval = millis() / 200;
    if (interval != newinterval) {
        interval = newinterval;
        vfd.setCursor(0, 0);
        if (brightness < 100) {
            vfd.clear();
            vfd.print("Brightness:");
            vfd.print(brightness);
            vfd.brightness(brightness);
            vfd.display(0);
        } else if (brightness == 101) {
            vfd.clear();
            vfd.print("display_off");
            vfd.display(0);
        } else if (brightness == 120) {
            vfd.brightness(0);
            vfd.clear();
            vfd.print("display_on");
            vfd.display(0);
        } else if (brightness == 130) {
            vfd.brightness(100);
        } else if (brightness > 140) {
            brightness = 0;
        }
        brightness = brightness + 1;
    }
}
