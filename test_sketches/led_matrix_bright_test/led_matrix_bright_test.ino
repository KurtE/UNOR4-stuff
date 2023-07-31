#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

void setup() {
  for (uint8_t i = 0; i < 96; i++) {
    matrix.on(i);
    delay(25);
  }
}

uint8_t on_times[12] = {4, 6, 8, 10, 12, 14, 16, 0, 0, 0, 0, 0};
void loop() {
  uint32_t start_time = micros();
  for (uint8_t i = 0; i < 12; i++) {
    if (on_times[i] != 0) {
      matrix.on(i+36);
      delayMicroseconds(on_times[i]);
    }
  }
  matrix.off(11+36);
  uint8_t tm0 = on_times[0];
  memmove(on_times, &on_times[1], 11);
  on_times[11] = tm0;
  uint32_t delta_time = micros() - start_time;
  if (delta_time < 1000) delayMicroseconds(1000 - delta_time);
  //  matrix.on(random(96));
  //  delay(100);
}