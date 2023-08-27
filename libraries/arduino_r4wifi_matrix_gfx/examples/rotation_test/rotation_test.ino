#include <arduino_r4wifi_matrix_gfx.h>
#include "font_Arial.h"

ArduinoLEDMatrixGFX display;
const float on_percents[] = { 100.0, 75.0, 50.0, 25.0, 12.5, 25.0, 50.0, 75.0 };

void setup() {
  while (!Serial && millis() < 5000)
    ;
  Serial.begin(9600);
  delay(1000);
  Serial.println("\n\nMatrix GFX class test program");
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

#ifdef MATRIX_INT_PER_PIXEL
  display.begin(1);
#else
  display.begin(10);
#endif
}

uint8_t rotation = 0;
char rot_str[] = "0";
void loop() {
  Serial.print("Test Rotation: ");
  Serial.println(rotation, DEC);
  display.setRotation(rotation);

  rot_str[0] = '0' + rotation;
  display.clearDisplay();
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(rot_str, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((display.width() - w) / 2, (display.height() - h) / 2);
  display.setTextColor(MATRIX_WHITE);
  display.print(rot_str);
  display.display();
  step();

  display.clearDisplay();
  display.drawRect(0, 0, display.width(), display.height(), MATRIX_WHITE);
  display.drawRect(1, 1, display.width() - 2, display.height() - 2, MATRIX_LIGHT);
  display.drawRect(2, 2, display.width() - 4, display.height() - 4, MATRIX_DARK);
  display.display();
  step();

  display.clearDisplay();
  for (uint8_t y = 0; y < display.height(); y += 2) {
    if ((y + 3) > display.width()) break;
    display.drawFastHLine(y, y, 3, MATRIX_WHITE);
  }
  display.display();
  step();

  display.clearDisplay();
  for (uint8_t x = 0; x < display.width(); x += 2) {
    if ((x + 3) > display.height()) break;
    display.drawFastVLine(x, x, 3, MATRIX_WHITE);
  }
  display.display();
  step();
  rotation = (rotation + 1) & 0x3;
}

void step() {
  Serial.println("*** Press any key to continue ***");
  while (Serial.read() == -1)
    ;
  while (Serial.read() != -1)
    ;
}
