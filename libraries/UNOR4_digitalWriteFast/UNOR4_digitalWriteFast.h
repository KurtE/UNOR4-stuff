/*
 *    UNOR4_digitalWriteFast.h - A quick and dirty digitalWriteFast
 *    and digitalToggleFast for Arduino UNO R4.  There are better
 *    versions out there, but this good enough for my testing
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
#ifndef _UNOR4_DIGITALWRITEFAST_H_
#define _UNOR4_DIGITALWRITEFAST_H_
#include <Arduino.h>
static R_PORT0_Type *port_table[] = { R_PORT0, R_PORT1, R_PORT2, R_PORT3, R_PORT4, R_PORT5, R_PORT6, R_PORT7 };
static const uint16_t mask_table[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7,
                                       1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15 };
// quick and dirty digitalWriteFast
static inline void digitalWriteFast(pin_size_t pin, PinStatus val) {
  uint16_t hardware_port_pin = g_pin_cfg[pin].pin;
  //uint16_t mask = 1 << (hardware_port_pin & 0xf);
  uint16_t pin_mask = mask_table[hardware_port_pin & 0xf];
  R_PORT0_Type *portX = port_table[hardware_port_pin >> 8];

  if (val) portX->POSR = pin_mask;
  else portX->PORR = pin_mask;
}
static inline void digitalToggleFast(pin_size_t pin) {
  uint16_t hardware_port_pin = g_pin_cfg[pin].pin;
  uint16_t pin_mask = mask_table[hardware_port_pin & 0xf];
  R_PORT0_Type *portX = port_table[hardware_port_pin >> 8];

  if (portX->PODR & pin_mask) portX->PORR = pin_mask;
  else portX->POSR = pin_mask;
}
#endif