#include "src/ACSReader.cpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET 4 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ACSReader acsr(5.06, A0, 3.2, 1023, 100);
int infoDelay = 0;


void setup() {
    Serial.begin(9600);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println("SSD1306 allocation failed!");
    }

    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();

    acsr.acs.autoMidPoint();
}

void loop() {
    float mA = acsr.getMedianDC(10, 100);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println("SSD1306 allocation failed");
    } else {
        char toPrint[21];
        snprintf(toPrint, 21, "A: %dmA\nW: %.02fW", (int)mA, acsr.getWatt());
        printDisplay(toPrint);
        delay(1000);

        if (infoDelay > 4) {
            snprintf(toPrint, 21, "Runtime:\n%s", acsr.getWorkingTime());
            printDisplay(toPrint);
            delay(3000);

            snprintf(toPrint, 21, "Supplied:\n%s", acsr.getSuppliedWattString());
            printDisplay(toPrint);
            delay(3000);

            infoDelay = 0;
        } else {
            infoDelay++;
        }
    }
}

void printDisplay(const char* str) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(str);
    display.display();
}
