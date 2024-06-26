#include <Arduino.h>
#include "M5Dial.h"

#define USE_USBPRINTF
// RGJ enabling USB debug printf causes loss of serial control for re-flash and reset/run, requiring btn-hold power cycle before can flash, and a power cycle after flash complete to run the code
//  Turning this off, after an initial btn-hold power cycle and flash, restores the ability to flash & reset/run automatically with no physical intervension
// WHY?  This has been working for months on the same HW with much larger builds until recently

// select debug output - serial, nothing or ...
#ifdef USE_USBPRINTF
#define DebugPrintLn(...) USBSerial.println(__VA_ARGS__) // USBSerial.println() works for m5Dial via USB C connection
#define DebugPrint(...) USBSerial.print(__VA_ARGS__)
#else
#define DebugPrintLn(...) // do nothing
#define DebugPrint(...)   // do nothing
#endif

TaskHandle_t Task2;


void Task2code(void *pvParameters)
{
  char buffer[90];

  for (;;)
  {
    delay(1700);

    snprintf(buffer, sizeof(buffer),
             "%lu: hello from Task2 running on core %d", millis(), xPortGetCoreID());
    DebugPrintLn(buffer);
  }
}

int interval = 1000;

void loop()
{
  unsigned long currentMillis;

  static unsigned long previousMillis = 0UL;
  currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {

    M5Dial.Display.fillCircle(120, 120, 100, WHITE);
    M5Dial.Display.fillCircle(120, 120, 50, RED);
    M5Dial.Display.setTextSize(3);
    M5Dial.Display.setTextColor(GREEN);
    M5Dial.Display.drawString("Hello World", 0, 120);
    previousMillis = currentMillis;
  }

#if 1 // 1s debug serial messages.  Also useful for remote dev where cant see screen
  {
    char buffer[90];
    static unsigned long startMillis;
    unsigned long currentMillis;
    currentMillis = millis();                // get the current "time" (actually the number of milliseconds since the program started)
    if (currentMillis - startMillis >= 1000) // 1000=1 second  test whether the period has elapsed
    {
      startMillis = currentMillis;

      snprintf(buffer, sizeof(buffer),
               "%lu: hello from loop() running on core %d", currentMillis, xPortGetCoreID());
      DebugPrintLn(buffer);
    }
  }
#endif
}

void setup()
{
  auto cfg = M5.config();
  M5Dial.begin(cfg, false, true);

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task2code, /* Task function. */
      "Task2",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task2,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  delay(500);

#ifdef USE_USBPRINTF
  USBSerial.begin(115200);
#endif
}
