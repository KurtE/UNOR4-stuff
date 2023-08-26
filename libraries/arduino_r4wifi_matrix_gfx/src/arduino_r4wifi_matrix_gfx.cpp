/*
 *    arduino_r4wifi_matrix_gfx.h - A library for controling the
 *    led matrix on the Arduino UNO R4 WIFI boards.
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

//#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define DBGPrintf printf
#else
inline void DBGPrintf(...) {}
#endif
#define NUM_LEDS    96

static const int pin_zero_index = 28;

static const uint8_t pins[][2] = {
  { 7, 3 }, { 3, 7 }, { 7, 4 }, { 4, 7 }, { 3, 4 }, { 4, 3 }, { 7, 8 }, { 8, 7 }, { 3, 8 }, { 8, 3 }, { 4, 8 }, { 8, 4 },
  { 7, 0 }, { 0, 7 }, { 3, 0 }, { 0, 3 }, { 4, 0 }, { 0, 4 }, { 8, 0 }, { 0, 8 }, { 7, 6 }, { 6, 7 }, { 3, 6 }, { 6, 3 },
  { 4, 6 }, { 6, 4 }, { 8, 6 }, { 6, 8 }, { 0, 6 }, { 6, 0 }, { 7, 5 }, { 5, 7 }, { 3, 5 }, { 5, 3 }, { 4, 5 }, { 5, 4 },
  { 8, 5 }, { 5, 8 }, { 0, 5 }, { 5, 0 }, { 6, 5 }, { 5, 6 }, { 7, 1 }, { 1, 7 }, { 3, 1 }, { 1, 3 }, { 4, 1 }, { 1, 4 },
  { 8, 1 }, { 1, 8 }, { 0, 1 }, { 1, 0 }, { 6, 1 }, { 1, 6 }, { 5, 1 }, { 1, 5 }, { 7, 2 }, { 2, 7 }, { 3, 2 }, { 2, 3 },
  { 4, 2 }, { 2, 4 }, { 8, 2 }, { 2, 8 }, { 0, 2 }, { 2, 0 }, { 6, 2 }, { 2, 6 }, { 5, 2 }, { 2, 5 }, { 1, 2 }, { 2, 1 },
  { 7, 10 }, { 10, 7 }, { 3, 10 }, { 10, 3 }, { 4, 10 }, { 10, 4 }, { 8, 10 }, { 10, 8 }, { 0, 10 }, { 10, 0 }, { 6, 10 }, { 10, 6 },
  { 5, 10 }, { 10, 5 }, { 1, 10 }, { 10, 1 }, { 2, 10 }, { 10, 2 }, { 7, 9 }, { 9, 7 }, { 3, 9 }, { 9, 3 }, { 4, 9 }, { 9, 4 }
};

static uint32_t volatile * const matrix_pin_pmnpfs[] = {
     &R_PFS->PORT[(g_pin_cfg[0 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[0 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[1 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[1 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[2 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[2 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[3 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[3 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[4 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[4 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[5 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[5 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[6 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[6 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[7 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[7 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[8 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[8 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[9 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[9 + pin_zero_index].pin) & 0xFF].PmnPFS,
     &R_PFS->PORT[(g_pin_cfg[10 + pin_zero_index].pin) >> 8].PIN[(g_pin_cfg[10 + pin_zero_index].pin) & 0xFF].PmnPFS
};


/*static*/ const int8_t matrix_irq_pins[11][10] = {
{50/*01*/, 64/*02*/, 15/*03*/, 17/*04*/, 38/*05*/ , 28/*06*/, 13/*07*/, 19/*08*/, 80/*10*/, -1},    // high 0
{51/*00*/, 70/*02*/, 45/*03*/, 47/*04*/, 55/*05*/, 53/*06*/, 43/*07*/, 49/*08*/, 86/*10*/, -1},     // High 1
{65/*00*/, 71/*01*/, 59/*03*/, 61/*04*/, 69/*05*/, 67/*06*/, 57/*07*/, 63/*08*/, 88/*10*/, -1},     // High 2
{14/*00*/, 44/*01*/, 58/*02*/, 4/*04*/, 32/*05*/ , 22/*06*/, 1/*07*/, 8/*08*/, 92/*09*/, 74/*10*/}, // High 3
{16/*00*/, 46/*01*/, 60/*02*/, 5/*03*/, 34/*05*/ , 24/*06*/, 3/*07*/, 10/*08*/, 94/*09*/, 76/*10*/},// High 4
{39/*00*/ , 54/*01*/, 68/*02*/, 33/*03*/ , 35/*04*/ , 41/*06*/, 31/*07*/, 37/*08*/ , 84/*10*/, -1}, // High 5
{29/*00*/, 52/*01*/, 66/*02*/, 23/*03*/, 25/*04*/, 40/*05*/, 21/*07*/, 27/*08*/, 82/*10*/, -1},     // High 6
{12/*00*/, 42/*01*/, 56/*02*/, 0/*03*/, 2/*04*/, 30/*05*/, 20/*06*/, 6/*08*/, 90/*09*/, 72/*10*/},  // High 7
{18/*00*/, 48/*01*/, 62/*02*/, 9/*03*/, 11/*04*/, 36/*05*/ , 26/*06*/, 7/*07*/, 78/*10*/, -1},      // High 8
{93/*03*/, 95/*04*/, 91/*07*/, -1},                                                                 // High 9
{81/*00*/, 87/*01*/, 89/*02*/, 75/*03*/, 77/*04*/, 85/*05*/, 83/*06*/, 73/*07*/, 79/*08*/, -1},     // High 10

};

#define LED_MATRIX_PORT0_MASK       ((1 << 3) | (1 << 4) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 15))
#define LED_MATRIX_PORT2_MASK       ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 12) | (1 << 13))

//=============================================================================
// Static members - should probably be most/all of them.
//=============================================================================
FspTimer ArduinoLEDMatrixGFX::s_ledTimer;
uint8_t __attribute__((aligned)) ArduinoLEDMatrixGFX::s_framebuffer[(NUM_LEDS * BITS_PER_PIXEL) / 8];
uint8_t ArduinoLEDMatrixGFX::s_high_pin_index = 0;
bool ArduinoLEDMatrixGFX::s_turn_off_pulse = false;
uint32_t ArduinoLEDMatrixGFX::s_rawPeriod = 0;
uint32_t ArduinoLEDMatrixGFX::s_rawPeriod1 = 0;
uint32_t ArduinoLEDMatrixGFX::s_rawPeriod2 = 0;
volatile uint32_t ArduinoLEDMatrixGFX::s_rawPeriodOn = 0;
volatile uint32_t ArduinoLEDMatrixGFX::s_rawPeriodOff = 0;
R_GPT0_Type *ArduinoLEDMatrixGFX::s_pgpt0 = nullptr;


ArduinoLEDMatrixGFX::ArduinoLEDMatrixGFX()
  : Adafruit_GFX(MATRIX_WIDTH, MATRIX_HEIGHT) {
  fillScreen(MATRIX_BLACK);
}

// destructor
ArduinoLEDMatrixGFX::~ArduinoLEDMatrixGFX() {
}

//=============================================================================
// Begin
//=============================================================================
bool ArduinoLEDMatrixGFX::begin(uint32_t frames_per_second) {
  uint8_t type;
  uint8_t ch = FspTimer::get_available_timer(type);

#ifdef MATRIX_INT_PER_PIXEL
  uint32_t frequency = frames_per_second * NUM_LEDS;
#else 
  uint32_t frequency = frames_per_second * 11;
#endif

  _pulseWidth =  1000000.0 / frequency;
  Serial.print("Freq: ");
  Serial.print(frequency, DEC);
  Serial.print(" Pulse Width:");
  Serial.println(_pulseWidth, DEC);
  Serial.print("ch:");
  Serial.print(ch, DEC);
  Serial.print(" type:");
  Serial.println(type, DEC);

  _new_period_on_percent = 0.0;
  s_ledTimer.begin(TIMER_MODE_PERIODIC, type, ch, _pulseWidth, 50.0, led_timer_callback, this);

  // lets get the pointer to the GPT object.
  s_pgpt0 = (R_GPT0_Type *)((uint32_t)R_GPT0 + ((uint32_t)R_GPT1 - (uint32_t)R_GPT0) * ch);
  // turn off GTPR Buffer 
  s_pgpt0->GTBER_b.PR = 0;
  s_pgpt0->GTBER_b.BD1 = 1;

  s_ledTimer.setup_overflow_irq();
  s_ledTimer.open();
  s_ledTimer.start();

  // Now lets see what the period;
  s_rawPeriod = s_ledTimer.get_period_raw();
  Serial.print("Period:");
  Serial.println(s_rawPeriod, DEC);

  s_rawPeriodOn = s_rawPeriod;
  s_rawPeriod1 = s_rawPeriod / 8;
  s_rawPeriod2 = s_rawPeriod / 4;
  s_rawPeriodOff = 0;
  return true;
}

void ArduinoLEDMatrixGFX::display() {
  memcpy(s_framebuffer, _frame_buffer, sizeof(_frame_buffer));
}

// virtual overrides - start simple
static inline void swap(int16_t &x, int16_t &y) {
  int16_t t = x;
  x = y;
  y = t;
}

inline void ArduinoLEDMatrixGFX::pixel(uint8_t pixel_index, uint16_t color) {
  // 2 bits per pixel
  uint8_t byte_index = pixel_index >> 2;
  uint8_t shift = (pixel_index & 0x3) * 2;
  _frame_buffer[byte_index] =  ((_frame_buffer[byte_index]) & ~(COLOR_MASK << shift)) 
      | (color & COLOR_MASK) << shift; 
}

uint16_t ArduinoLEDMatrixGFX::pixelDisp(uint8_t pixel_index) {
  // 2 bits per pixel
  uint8_t byte_index = pixel_index >> 2;
  uint8_t shift = (pixel_index & 0x3) * 2;
  return (s_framebuffer[byte_index] >> shift) & COLOR_MASK;
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
  
  pixel(y * MATRIX_WIDTH + x, color);
}

void ArduinoLEDMatrixGFX::fillScreen(uint16_t color) {
  if (color == MATRIX_WHITE) {
    memset(_frame_buffer, 0xff, sizeof(_frame_buffer));
  } else {
    memset(_frame_buffer, 0x00, sizeof(_frame_buffer));
  }
}

void ArduinoLEDMatrixGFX::clearDisplay() {
  fillScreen(MATRIX_BLACK);
}

void ArduinoLEDMatrixGFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  DBGPrintf("\t\t\tFHline(%d, %d, %d, %x ->", x, y, w, textcolor);
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
  DBGPrintf("\t\t\tFVline(%d, %d, %d, %x ->", x, y, h, textcolor);
  bool swapped = false;
  switch (getRotation()) {
    case 1:
      swap(x, y);
      swapped = true;
      x = MATRIX_WIDTH - (x + h);
      break;
    case 2:
      x = MATRIX_WIDTH - x - 1;
      y = MATRIX_HEIGHT - (y+h);
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
  DBGPrintf("hline(%d, %d, %d, %x\n", x, y, w, textcolor);

  if ((y < 0 ) || (y >= MATRIX_HEIGHT)) return;
  int16_t x_end = x + w;
  if (x < 0) x = 0;
  if (x_end > MATRIX_WIDTH) x_end = MATRIX_WIDTH;

  uint8_t pixel_index = y * MATRIX_WIDTH + x;
  while (x < x_end) {
    pixel(pixel_index, color);
    x++;
    pixel_index++;
  }
}

void ArduinoLEDMatrixGFX::vline(int16_t x, int16_t y, int16_t h, uint16_t color) {
  DBGPrintf("vline(%d, %d, %d, %x\n", x, y, h, textcolor);
  if ((x < 0 ) || (x >= MATRIX_WIDTH)) return;
  int16_t y_end = y + h;
  if (y < 0) y = 0;
  if (y_end > MATRIX_HEIGHT) y_end = MATRIX_HEIGHT;

  uint8_t pixel_index = y * MATRIX_WIDTH + x;
  while (y < y_end) {
    pixel(pixel_index, color);
    y++;
    pixel_index += MATRIX_WIDTH;
  }
}

void ArduinoLEDMatrixGFX::updateClockPeriod(uint32_t period) {
  if (s_pgpt0) s_pgpt0->GTPR = period;
  else s_ledTimer.set_period(period);
}


//=================================================================================
// Test to support ILI ilifont code
//=================================================================================

// See if we can overwrite the print write method
static uint32_t fetchbit(const uint8_t *p, uint32_t index) {
  if (p[index >> 3] & (1 << (7 - (index & 7))))
    return 1;
  return 0;
}

static uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index,
                                   uint32_t required) {
  uint32_t val = 0;
  do {
    uint8_t b = p[index >> 3];
    uint32_t avail = 8 - (index & 7);
    if (avail <= required) {
      val <<= avail;
      val |= b & ((1 << avail) - 1);
      index += avail;
      required -= avail;
    } else {
      b >>= avail - required;
      val <<= required;
      val |= b & ((1 << required) - 1);
      break;
    }
  } while (required);
  return val;
}

static uint32_t fetchbits_signed(const uint8_t *p, uint32_t index,
                                 uint32_t required) {
  uint32_t val = fetchbits_unsigned(p, index, required);
  if (val & (1 << (required - 1))) {
    return (int32_t)val - (1 << required);
  }
  return (int32_t)val;
}


size_t ArduinoLEDMatrixGFX::write(uint8_t c) {

  if (!ilifont) return Adafruit_GFX::write(c);

    uint32_t bitoffset;
  const uint8_t *data;

  DBGPrintf("drawFontChar %d\n", c);

  if (c >= ilifont->index1_first && c <= ilifont->index1_last) {
    bitoffset = c - ilifont->index1_first;
    bitoffset *= ilifont->bits_index;
  } else if (c >= ilifont->index2_first && c <= ilifont->index2_last) {
    bitoffset = c - ilifont->index2_first + ilifont->index1_last - ilifont->index1_first + 1;
    bitoffset *= ilifont->bits_index;
  } else {
    return 0;
  }
  DBGPrintf("  index =  %d\n", fetchbits_unsigned(ilifont->index, bitoffset, ilifont->bits_index));
  data = ilifont->data + fetchbits_unsigned(ilifont->index, bitoffset, ilifont->bits_index);

  uint32_t encoding = fetchbits_unsigned(data, 0, 3);
  if (encoding != 0) return 0;
  uint32_t width = fetchbits_unsigned(data, 3, ilifont->bits_width);
  bitoffset = ilifont->bits_width + 3;
  uint32_t height = fetchbits_unsigned(data, bitoffset, ilifont->bits_height);
  bitoffset += ilifont->bits_height;
  DBGPrintf("  size =   %d,%d\n", width, height);

  int32_t xoffset = fetchbits_signed(data, bitoffset, ilifont->bits_xoffset);
  bitoffset += ilifont->bits_xoffset;
  int32_t yoffset = fetchbits_signed(data, bitoffset, ilifont->bits_yoffset);
  bitoffset += ilifont->bits_yoffset;
  DBGPrintf("  offset = %d,%d\n", xoffset, yoffset);

  uint32_t delta = fetchbits_unsigned(data, bitoffset, ilifont->bits_delta);
  bitoffset += ilifont->bits_delta;
  DBGPrintf("  delta =  %d\n", delta);

  DBGPrintf("  cursor = %d,%d\n", cursor_x, cursor_y);

  // horizontally, we draw every pixel, or none at all
  if (cursor_x < 0) cursor_x = 0;
  int32_t origin_x = cursor_x + xoffset;
  if (origin_x < 0) {
    cursor_x -= xoffset;
    origin_x = 0;
  }
  if (origin_x + (int)width > _width) {
    // I am going to allow partial characters to be output
    if (wrap) {
      origin_x = 0;
      if (xoffset >= 0) {
        cursor_x = 0;
      } else {
        cursor_x = -xoffset;
      }
      cursor_y += ilifont->line_space;
    }
  }
  //if (cursor_y >= _height) return 1;
  cursor_x += delta;

  // vertically, the top and/or bottom can be clipped
  int32_t origin_y = cursor_y + ilifont->cap_height - height - yoffset;
  DBGPrintf("  cy:%d cap:%d height:%d yof:%d\n", cursor_y, ilifont->cap_height, height, yoffset);
  DBGPrintf("  origin = %d,%d\n", origin_x, origin_y);

  // TODO: compute top skip and number of lines
  int32_t linecount = height;
  uint32_t loopcount = 0;
  uint32_t y = origin_y;
  while (linecount) {
    DBGPrintf("    linecount = %d\n", linecount);
    uint32_t b = fetchbit(data, bitoffset++);
    if (b == 0) {
      DBGPrintf("    single line\n");
      uint32_t x = 0;
      do {
        uint32_t xsize = width - x;
        if (xsize > 32) xsize = 32;
        uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
        drawFontBits(bits, xsize, origin_x + x, y, 1);
        bitoffset += xsize;
        x += xsize;
      } while (x < width);
      y++;
      linecount--;
    } else {
      uint32_t n = fetchbits_unsigned(data, bitoffset, 3) + 2;
      bitoffset += 3;
      uint32_t x = 0;
      do {
        uint32_t xsize = width - x;
        if (xsize > 32) xsize = 32;
        DBGPrintf("    multi line %d\n", n);
        uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
        drawFontBits(bits, xsize, origin_x + x, y, n);
        bitoffset += xsize;
        x += xsize;
      } while (x < width);
      y += n;
      linecount -= n;
    }
    //if (++loopcount > 100) {
      //Serial.println("     abort draw loop");
      //break;
    //}
  }
  return 1;
}

void ArduinoLEDMatrixGFX::drawFontBits(uint32_t bits, uint32_t numbits, uint32_t x, uint32_t y, uint32_t repeat)
{
  if (bits == 0) return;
  uint32_t w;
  bits <<= (32-numbits); // left align bits 
  do {
    w = __builtin_clz(bits); // skip over zeros
    if (w > numbits) w = numbits;
    numbits -= w;
    x += w;
    bits <<= w;
    bits = ~bits; // invert to count 1s as 0s
    w = __builtin_clz(bits);
    if (w > numbits) w = numbits; 
    numbits -= w;
    bits <<= w;
    bits = ~bits; // invert back to original polarity
    if (w > 0) {
      DBGPrintf("\t\tfillRect(%u, %u, %u, %u, %x\n", x, y, w, repeat, textcolor);
      fillRect(x, y, w, repeat, textcolor);
      x += w;
    }
  } while (numbits > 0);
}



//=================================================================================
// Timer ISR Callback from 
//=================================================================================
void ArduinoLEDMatrixGFX::led_timer_callback(timer_callback_args_t *arg) {
    if (arg == nullptr || arg->p_context == nullptr) return; // ???

    // turn all of the pins to input.
    digitalWriteFast(3, LOW); 
    R_PORT0->PCNTR1 &= ~((uint32_t) LED_MATRIX_PORT0_MASK);
    R_PORT2->PCNTR1 &= ~((uint32_t) LED_MATRIX_PORT2_MASK);
#ifdef MATRIX_INT_PER_PIXEL
    static volatile int i_isr = 0;
    // See if 
    if (s_rawPeriodOff) {
        // only pupose was to turn off the leds
        updateClockPeriod(s_rawPeriodOff); // how long it will off
        s_rawPeriodOff = 0;
        return;
    }        
    uint8_t pixel_color = pixelDisp(i_isr);
    uint32_t lastRawPeriodOn = s_rawPeriodOn; 

    if (pixel_color != 0) {
      digitalWriteFast(3, HIGH); 
      *matrix_pin_pmnpfs[pins[i_isr][0]] = IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH;
      *matrix_pin_pmnpfs[pins[i_isr][1]] = IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_LOW;
      switch (pixel_color) {
        default: s_rawPeriodOn = s_rawPeriod; break;
        case 1: s_rawPeriodOn = s_rawPeriod1; break;
        case 2: s_rawPeriodOn = s_rawPeriod2; break;  
      }
    } else {
      s_rawPeriodOn = s_rawPeriod;
    }

    if ((s_rawPeriodOn != s_rawPeriod) || (lastRawPeriodOn != s_rawPeriod)) {
       updateClockPeriod(s_rawPeriodOn); // how long it will off
    }
    s_rawPeriodOff = s_rawPeriod - s_rawPeriodOn;
    i_isr = (i_isr + 1) % NUM_LEDS;

#else
    ArduinoLEDMatrixGFX* _m   = (ArduinoLEDMatrixGFX*)arg->p_context;
    if (s_turn_off_pulse) {
        // only pupose was to turn off the leds
        updateClockPeriod(s_rawPeriodOff); // how long it will off
        s_turn_off_pulse = false;
        return;
    }        

    bool first_on = true;
    for (uint8_t i = 0; i < 10; i++) {
      uint8_t led = matrix_irq_pins[s_high_pin_index][i];
      if (led == (uint8_t)-1) break;
      if (pixelDisp(led) != 0) {
        if (first_on) {
          *matrix_pin_pmnpfs[s_high_pin_index] = IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH;
          first_on = false;
         digitalWriteFast(3, HIGH); 
        }
        *matrix_pin_pmnpfs[pins[led][1]] = IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_LOW;
      }
    }
    if (s_rawPeriodOff) {
        if (!first_on) {
            updateClockPeriod(s_rawPeriodOn); // Setup for how long the LED should be on
            s_turn_off_pulse = true;

        } else {
            updateClockPeriod(s_rawPeriod); // no pins activated so wait full pulse width            
        }
    }
    s_high_pin_index++;
    if (s_high_pin_index == 11) {
        s_high_pin_index = 0;
        if (_m->_new_period_on_percent != 0.0) {
            if (_m->_new_period_on_percent  > 99.9) {
                s_rawPeriodOn = s_rawPeriod;
                s_rawPeriodOff = 0;                    
                updateClockPeriod(s_rawPeriod); // how long it will off
            } else {
                s_rawPeriodOn = (float)s_rawPeriod * _m->_new_period_on_percent / 100.0;
                // make sure there is at least some on time... 
                constrain(s_rawPeriodOn, 32, s_rawPeriod);
                s_rawPeriodOff = s_rawPeriod - s_rawPeriodOn;
                updateClockPeriod(s_rawPeriodOn); // how long it will off
                s_turn_off_pulse = true; 
            }
            //Serial.print("NP: ");
            //Serial.print(_m->_new_period_on_percent, 2);
            //Serial.print(" On:");
            //Serial.print(s_rawPeriodOn);
            //Serial.print(" Off:");
            //Serial.println(s_rawPeriodOff);
            _m->_new_period_on_percent = 0.0;
        }
    }
#endif
}


