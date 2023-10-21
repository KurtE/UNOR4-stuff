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
#ifndef _GIGA_DIGITALWRITEFAST_H_
#define _GIGA_DIGITALWRITEFAST_H_
#include <Arduino.h>
#include "pinDefinitions.h"
#if !defined(ARDUINO_GIGA)
#error "Only works on Arduino GIGA boards"
#endif


static  GPIO_TypeDef * const port_table[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK };
static const uint16_t mask_table[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7,
                                       1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15 };

// quick and dirty digitalWriteFast
static inline void digitalWriteFast(uint8_t pin, PinStatus val) __attribute__((always_inline, unused));
static inline void digitalWriteFast(pin_size_t pin, PinStatus val) {
  PinName hardware_port_pin = g_APinDescription[pin].name;
  //uint16_t mask = 1 << (hardware_port_pin & 0xf);
  uint16_t mask = mask_table[hardware_port_pin & 0xf];
  GPIO_TypeDef  * const port = port_table[hardware_port_pin >> 4];
  if (val) port->BSRR = mask;
  else port->BSRR = (uint32_t)(mask << 16);
}


static inline void digitalToggleFast(uint8_t pin) __attribute__((always_inline, unused));
static inline void digitalToggleFast(pin_size_t pin) {
  PinName hardware_port_pin = g_APinDescription[pin].name;
  uint16_t pin_mask = mask_table[hardware_port_pin & 0xf];
  GPIO_TypeDef  * const portX = port_table[hardware_port_pin >> 4];

  if (portX->ODR & pin_mask) portX->BSRR = (uint32_t)(pin_mask << 16);
  else portX->BSRR = pin_mask;
}

static inline uint16_t digitalReadFast(pin_size_t pin) {
  PinName hardware_port_pin = g_APinDescription[pin].name;
  uint16_t pin_mask = mask_table[hardware_port_pin & 0xf];
  GPIO_TypeDef  * const portX = port_table[hardware_port_pin >> 4];

  return (portX->IDR & pin_mask);
}

#endif