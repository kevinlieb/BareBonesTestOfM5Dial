#include <Arduino.h>
#include "M5Dial.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
    auto cfg = M5.config();
    M5Dial.begin(cfg, false, true);
}

int interval = 1000;

void loop() {
  unsigned long currentMillis;

  static unsigned long previousMillis = 0UL;
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {

    M5Dial.Display.fillCircle(120, 120, 100, WHITE);
    M5Dial.Display.fillCircle(120, 120, 50, RED);
    M5Dial.Display.setTextSize(3);
    M5Dial.Display.setTextColor(GREEN);
    M5Dial.Display.drawString("Hello World", 0, 120);
    previousMillis = currentMillis;
  }



}

