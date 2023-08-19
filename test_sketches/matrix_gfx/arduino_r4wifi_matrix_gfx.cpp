#include "arduino_r4wifi_matrix_gfx.h"

ArduinoLEDMatrix matrix;


ArduinoLEDMatrixGFX::ArduinoLEDMatrixGFX()
  : Adafruit_GFX(MATRIX_WIDTH, MATRIX_HEIGHT) {
  fillScreen(MATRIX_BLACK);
}

// destructor
ArduinoLEDMatrixGFX::~ArduinoLEDMatrixGFX() {
}

bool ArduinoLEDMatrixGFX::begin() {
  matrix.begin();
  return true;
}

void ArduinoLEDMatrixGFX::display() {
  matrix.loadFrame(_frame_buffer);
}

// virtual overrides - start simple
static inline void swap(int16_t x, int16_t y) {
  int16_t t = x;
  x = y;
  y = t;
}

void ArduinoLEDMatrixGFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
  switch (getRotation()) {
    case 1:
      swap(x, y);
      x = MATRIX_WIDTH - x - 1;
      break;
    case 2:
      x = MATRIX_WIDTH - x - 1;
      y = MATRIX_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = MATRIX_HEIGHT - y - 1;
      break;
  }
  if ((x < 0 ) || (x >= MATRIX_WIDTH)) return;
  if ((y < 0 ) || (y >= MATRIX_HEIGHT)) return;
  
  uint8_t pixel = y * MATRIX_WIDTH + x;

  switch (color) {
    case MATRIX_WHITE:
      _frame_buffer[pixel >> 5] |= (1 << (31 - (pixel & 0x1f)));
      break;
    case MATRIX_BLACK:
      _frame_buffer[pixel >> 5] &= ~(1 << (31 - (pixel & 0x1f)));
      break;
    case MATRIX_INVERSE:
      _frame_buffer[pixel >> 5] ^= (1 << (31 - (pixel & 0x1f)));
      break;
  }
}

void ArduinoLEDMatrixGFX::fillScreen(uint16_t color) {
  if (color == MATRIX_WHITE) {
    _frame_buffer[0] = _frame_buffer[1] = _frame_buffer[2] = 0xffffffff;
  } else {
    _frame_buffer[0] = _frame_buffer[1] = _frame_buffer[2] = 0x00000000;
  }
}

void ArduinoLEDMatrixGFX::clearDisplay() {
  fillScreen(MATRIX_BLACK);
}
