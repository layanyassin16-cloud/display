#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 1);
const int pirPin = 2;

uint64_t imageIdle = 0x7e8100424a1899fd;
uint64_t imageMotion = 0x7e8100e7081899fd;

// Timer variables for the motion hold duration
unsigned long lastMotionTime = 0;
const unsigned long holdDelay = 3000; // Keeps animation going for 3,000ms (3 seconds) after motion stops

void setup() {
  pinMode(pirPin, INPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void loop() {
  int motionState = digitalRead(pirPin);

  if (motionState == HIGH) {
    // Every time motion is detected, update the timestamp
    lastMotionTime = millis();
  }

  // If current time is within 3 seconds of the last motion event, keep animating
  if (millis() - lastMotionTime < holdDelay) {
    displayImage(imageMotion);
    delay(300);
    displayImage(imageIdle);
    delay(300);
  } else {
    // Otherwise, go back to theidle state
    displayImage(imageIdle);
  }
}

void displayImage(uint64_t img) {
  for (int i = 0; i < 8; i++) {
    byte row = (img >> (i * 8)) & 0xFF;
    lc.setRow(0, i, row);
  }
}