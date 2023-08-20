/*
 *    arduino_r4wifi_matrix_gfx.h - A library for controling the
 *    lec matrix on the Arduino UNO R4 WIFI boards.
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
*/
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

void ArduinoLEDMatrixGFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  bool swapped = false;
  switch (getRotation()) {
    case 1:
      swap(x, y);
      swapped = true;
      x = MATRIX_WIDTH - x - 1;
      break;
    case 2:
      x = MATRIX_WIDTH - x - 1;
      y = MATRIX_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      swapped = true;
      y = MATRIX_HEIGHT - y - 1;
      break;
  }

  if (swapped) vline(x, y, w, color);
  else hline(x, y, w, color);
}

void ArduinoLEDMatrixGFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  bool swapped = false;
  switch (getRotation()) {
    case 1:
      swap(x, y);
      swapped = true;
      x = MATRIX_WIDTH - x - 1;
      break;
    case 2:
      x = MATRIX_WIDTH - x - 1;
      y = MATRIX_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      swapped = true;
      y = MATRIX_HEIGHT - y - 1;
      break;
  }

  if (swapped) hline(x, y, h, color);
  else vline(x, y, h, color);

}


void ArduinoLEDMatrixGFX::hline(int16_t x, int16_t y, int16_t w, uint16_t color) {
  if ((y < 0 ) || (y >= MATRIX_HEIGHT)) return;
  int16_t x_end = x + w;
  if (x < 0) x = 0;
  if (x_end > MATRIX_WIDTH) x_end = MATRIX_WIDTH;

  uint8_t pixel = y * MATRIX_WIDTH + x;
  while (x < x_end) {
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
    x++;
    pixel++;
  }
}

void ArduinoLEDMatrixGFX::vline(int16_t x, int16_t y, int16_t h, uint16_t color) {
  if ((x < 0 ) || (x >= MATRIX_WIDTH)) return;
  int16_t y_end = y + h;
  if (y < 0) y = 0;
  if (y_end > MATRIX_HEIGHT) y_end = MATRIX_HEIGHT;

  uint8_t pixel = y * MATRIX_WIDTH + x;
  while (y < y_end) {
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
    y++;
    pixel += MATRIX_WIDTH;
  }
}



