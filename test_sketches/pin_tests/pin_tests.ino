static inline void digitalWriteFast(uint8_t pin, uint8_t val) __attribute__((always_inline, unused));
static inline void fasterDigitalWrite(uint8_t pin, uint8_t val) __attribute__((always_inline, unused));

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nTest");
  while (!Serial && millis() < 5000)
    ;

  pinMode(LED_BUILTIN, OUTPUT);
  uint32_t start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }
  uint32_t delta_time = micros() - start_time;
  Serial.print("digitalWrite: ");
  Serial.println(delta_time, DEC);

  start_time = micros();
  for (int i = 0; i < 1000; i++) {
    fasterDigitalWrite(LED_BUILTIN, HIGH);
    fasterDigitalWrite(LED_BUILTIN, LOW);
  }

  delta_time = micros() - start_time;
  Serial.print("Faster: ");
  Serial.println(delta_time, DEC);

  start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalWriteFast(LED_BUILTIN, HIGH);
    digitalWriteFast(LED_BUILTIN, LOW);
  }

  delta_time = micros() - start_time;
  Serial.print("digitalWriteFast: ");
  Serial.println(delta_time, DEC);


  start_time = micros();
  for (int i = 0; i < 1000; i++) {
    R_PORT1->POSR = bit(2);
    R_PORT1->PORR = bit(2);
  }
  delta_time = micros() - start_time;
  Serial.print("POSR/PORR: ");
  Serial.println(delta_time, DEC);
}

void loop() {
}


R_PORT0_Type *port_table[] = { R_PORT0, R_PORT1, R_PORT2, R_PORT3, R_PORT4, R_PORT5, R_PORT6, R_PORT7 };
static const uint16_t mask_table[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7,
                                       1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15 };


static inline void  fasterDigitalWrite(pin_size_t pin, PinStatus val) {
  uint16_t hardware_port_pin = g_pin_cfg[pin].pin;
  //uint16_t mask = 1 << (hardware_port_pin & 0xf);
  uint16_t mask = mask_table[hardware_port_pin & 0xf];
  R_PORT0_Type *port = port_table[hardware_port_pin >> 8];
  if (val) port->POSR = mask;
  else port->PORR = mask;
}

// Cause a digital pin to output either HIGH or LOW.  The pin must
// have been configured with pinMode().  This fast version of
// digitalWrite has minimal overhead when the pin number is a
// constant.  Successive digitalWriteFast without delays can be
// too quick in many applications!
static inline void digitalWriteFast(uint8_t pin, uint8_t val) {
  if (__builtin_constant_p(pin)) {
    if (val) {
      if (pin == 0) {
        port_table[g_pin_cfg[0].pin >> 8]->POSR = mask_table[g_pin_cfg[0].pin & 0xff];
      } else if (pin == 1) {
        port_table[g_pin_cfg[1].pin >> 8]->POSR = mask_table[g_pin_cfg[1].pin & 0xff];
      } else if (pin == 2) {
        port_table[g_pin_cfg[2].pin >> 8]->POSR = mask_table[g_pin_cfg[2].pin & 0xff];
      } else if (pin == 3) {
        port_table[g_pin_cfg[3].pin >> 8]->POSR = mask_table[g_pin_cfg[3].pin & 0xff];
      } else if (pin == 4) {
        port_table[g_pin_cfg[4].pin >> 8]->POSR = mask_table[g_pin_cfg[4].pin & 0xff];
      } else if (pin == 5) {
        port_table[g_pin_cfg[5].pin >> 8]->POSR = mask_table[g_pin_cfg[5].pin & 0xff];
      } else if (pin == 6) {
        port_table[g_pin_cfg[6].pin >> 8]->POSR = mask_table[g_pin_cfg[6].pin & 0xff];
      } else if (pin == 7) {
        port_table[g_pin_cfg[7].pin >> 8]->POSR = mask_table[g_pin_cfg[7].pin & 0xff];
      } else if (pin == 8) {
        port_table[g_pin_cfg[8].pin >> 8]->POSR = mask_table[g_pin_cfg[8].pin & 0xff];
      } else if (pin == 9) {
        port_table[g_pin_cfg[9].pin >> 8]->POSR = mask_table[g_pin_cfg[9].pin & 0xff];
      } else if (pin == 10) {
        port_table[g_pin_cfg[10].pin >> 8]->POSR = mask_table[g_pin_cfg[10].pin & 0xff];
      } else if (pin == 11) {
        port_table[g_pin_cfg[11].pin >> 8]->POSR = mask_table[g_pin_cfg[11].pin & 0xff];
      } else if (pin == 12) {
        port_table[g_pin_cfg[12].pin >> 8]->POSR = mask_table[g_pin_cfg[12].pin & 0xff];
      } else if (pin == 13) {
        port_table[g_pin_cfg[13].pin >> 8]->POSR = mask_table[g_pin_cfg[13].pin & 0xff];
      } else if (pin == 14) {
        port_table[g_pin_cfg[14].pin >> 8]->POSR = mask_table[g_pin_cfg[14].pin & 0xff];
      } else if (pin == 15) {
        port_table[g_pin_cfg[15].pin >> 8]->POSR = mask_table[g_pin_cfg[15].pin & 0xff];
      } else if (pin == 16) {
        port_table[g_pin_cfg[16].pin >> 8]->POSR = mask_table[g_pin_cfg[16].pin & 0xff];
      } else if (pin == 17) {
        port_table[g_pin_cfg[17].pin >> 8]->POSR = mask_table[g_pin_cfg[17].pin & 0xff];
      } else if (pin == 18) {
        port_table[g_pin_cfg[18].pin >> 8]->POSR = mask_table[g_pin_cfg[18].pin & 0xff];
      } else if (pin == 19) {
        port_table[g_pin_cfg[19].pin >> 8]->POSR = mask_table[g_pin_cfg[19].pin & 0xff];
      } else if (pin == 20) {
        port_table[g_pin_cfg[20].pin >> 8]->POSR = mask_table[g_pin_cfg[20].pin & 0xff];
      } else if (pin == 21) {
        port_table[g_pin_cfg[21].pin >> 8]->POSR = mask_table[g_pin_cfg[21].pin & 0xff];
      } else if (pin == 22) {
        port_table[g_pin_cfg[22].pin >> 8]->POSR = mask_table[g_pin_cfg[22].pin & 0xff];
      } else if (pin == 23) {
        port_table[g_pin_cfg[23].pin >> 8]->POSR = mask_table[g_pin_cfg[23].pin & 0xff];
      } else if (pin == 24) {
        port_table[g_pin_cfg[24].pin >> 8]->POSR = mask_table[g_pin_cfg[24].pin & 0xff];
      } else if (pin == 25) {
        port_table[g_pin_cfg[25].pin >> 8]->POSR = mask_table[g_pin_cfg[25].pin & 0xff];
      } else if (pin == 26) {
        port_table[g_pin_cfg[26].pin >> 8]->POSR = mask_table[g_pin_cfg[26].pin & 0xff];
      } else if (pin == 27) {
        port_table[g_pin_cfg[27].pin >> 8]->POSR = mask_table[g_pin_cfg[27].pin & 0xff];
      } else if (pin == 28) {
        port_table[g_pin_cfg[28].pin >> 8]->POSR = mask_table[g_pin_cfg[28].pin & 0xff];
      } else if (pin == 29) {
        port_table[g_pin_cfg[29].pin >> 8]->POSR = mask_table[g_pin_cfg[29].pin & 0xff];
      } else if (pin == 30) {
        port_table[g_pin_cfg[30].pin >> 8]->POSR = mask_table[g_pin_cfg[30].pin & 0xff];
      } else if (pin == 31) {
        port_table[g_pin_cfg[31].pin >> 8]->POSR = mask_table[g_pin_cfg[31].pin & 0xff];
      } else if (pin == 32) {
        port_table[g_pin_cfg[32].pin >> 8]->POSR = mask_table[g_pin_cfg[32].pin & 0xff];
      } else if (pin == 33) {
        port_table[g_pin_cfg[33].pin >> 8]->POSR = mask_table[g_pin_cfg[33].pin & 0xff];
      } else if (pin == 34) {
        port_table[g_pin_cfg[34].pin >> 8]->POSR = mask_table[g_pin_cfg[34].pin & 0xff];
      } else if (pin == 35) {
        port_table[g_pin_cfg[35].pin >> 8]->POSR = mask_table[g_pin_cfg[35].pin & 0xff];
      } else if (pin == 36) {
        port_table[g_pin_cfg[36].pin >> 8]->POSR = mask_table[g_pin_cfg[36].pin & 0xff];
      } else if (pin == 37) {
        port_table[g_pin_cfg[37].pin >> 8]->POSR = mask_table[g_pin_cfg[37].pin & 0xff];
      } else if (pin == 38) {
        port_table[g_pin_cfg[38].pin >> 8]->POSR = mask_table[g_pin_cfg[38].pin & 0xff];
      }
    } else {
      if (pin == 0) {
        port_table[g_pin_cfg[0].pin >>8]->PORR = mask_table[g_pin_cfg[0].pin & 0xff];
      } else if (pin == 1) {
        port_table[g_pin_cfg[1].pin >>8]->PORR = mask_table[g_pin_cfg[1].pin & 0xff];
      } else if (pin == 2) {
        port_table[g_pin_cfg[2].pin >>8]->PORR = mask_table[g_pin_cfg[2].pin & 0xff];
      } else if (pin == 3) {
        port_table[g_pin_cfg[3].pin >>8]->PORR = mask_table[g_pin_cfg[3].pin & 0xff];
      } else if (pin == 4) {
        port_table[g_pin_cfg[4].pin >>8]->PORR = mask_table[g_pin_cfg[4].pin & 0xff];
      } else if (pin == 5) {
        port_table[g_pin_cfg[5].pin >>8]->PORR = mask_table[g_pin_cfg[5].pin & 0xff];
      } else if (pin == 6) {
        port_table[g_pin_cfg[6].pin >>8]->PORR = mask_table[g_pin_cfg[6].pin & 0xff];
      } else if (pin == 7) {
        port_table[g_pin_cfg[7].pin >>8]->PORR = mask_table[g_pin_cfg[7].pin & 0xff];
      } else if (pin == 8) {
        port_table[g_pin_cfg[8].pin >>8]->PORR = mask_table[g_pin_cfg[8].pin & 0xff];
      } else if (pin == 9) {
        port_table[g_pin_cfg[9].pin >>8]->PORR = mask_table[g_pin_cfg[9].pin & 0xff];
      } else if (pin == 10) {
        port_table[g_pin_cfg[10].pin >>8]->PORR = mask_table[g_pin_cfg[10].pin & 0xff];
      } else if (pin == 11) {
        port_table[g_pin_cfg[11].pin >>8]->PORR = mask_table[g_pin_cfg[11].pin & 0xff];
      } else if (pin == 12) {
        port_table[g_pin_cfg[12].pin >>8]->PORR = mask_table[g_pin_cfg[12].pin & 0xff];
      } else if (pin == 13) {
        port_table[g_pin_cfg[13].pin >>8]->PORR = mask_table[g_pin_cfg[13].pin & 0xff];
      } else if (pin == 14) {
        port_table[g_pin_cfg[14].pin >>8]->PORR = mask_table[g_pin_cfg[14].pin & 0xff];
      } else if (pin == 15) {
        port_table[g_pin_cfg[15].pin >>8]->PORR = mask_table[g_pin_cfg[15].pin & 0xff];
      } else if (pin == 16) {
        port_table[g_pin_cfg[16].pin >>8]->PORR = mask_table[g_pin_cfg[16].pin & 0xff];
      } else if (pin == 17) {
        port_table[g_pin_cfg[17].pin >>8]->PORR = mask_table[g_pin_cfg[17].pin & 0xff];
      } else if (pin == 18) {
        port_table[g_pin_cfg[18].pin >>8]->PORR = mask_table[g_pin_cfg[18].pin & 0xff];
      } else if (pin == 19) {
        port_table[g_pin_cfg[19].pin >>8]->PORR = mask_table[g_pin_cfg[19].pin & 0xff];
      } else if (pin == 20) {
        port_table[g_pin_cfg[20].pin >>8]->PORR = mask_table[g_pin_cfg[20].pin & 0xff];
      } else if (pin == 21) {
        port_table[g_pin_cfg[21].pin >>8]->PORR = mask_table[g_pin_cfg[21].pin & 0xff];
      } else if (pin == 22) {
        port_table[g_pin_cfg[22].pin >>8]->PORR = mask_table[g_pin_cfg[22].pin & 0xff];
      } else if (pin == 23) {
        port_table[g_pin_cfg[23].pin >>8]->PORR = mask_table[g_pin_cfg[23].pin & 0xff];
      } else if (pin == 24) {
        port_table[g_pin_cfg[24].pin >>8]->PORR = mask_table[g_pin_cfg[24].pin & 0xff];
      } else if (pin == 25) {
        port_table[g_pin_cfg[25].pin >>8]->PORR = mask_table[g_pin_cfg[25].pin & 0xff];
      } else if (pin == 26) {
        port_table[g_pin_cfg[26].pin >>8]->PORR = mask_table[g_pin_cfg[26].pin & 0xff];
      } else if (pin == 27) {
        port_table[g_pin_cfg[27].pin >>8]->PORR = mask_table[g_pin_cfg[27].pin & 0xff];
      } else if (pin == 28) {
        port_table[g_pin_cfg[28].pin >>8]->PORR = mask_table[g_pin_cfg[28].pin & 0xff];
      } else if (pin == 29) {
        port_table[g_pin_cfg[29].pin >>8]->PORR = mask_table[g_pin_cfg[29].pin & 0xff];
      } else if (pin == 30) {
        port_table[g_pin_cfg[30].pin >>8]->PORR = mask_table[g_pin_cfg[30].pin & 0xff];
      } else if (pin == 31) {
        port_table[g_pin_cfg[31].pin >>8]->PORR = mask_table[g_pin_cfg[31].pin & 0xff];
      } else if (pin == 32) {
        port_table[g_pin_cfg[32].pin >>8]->PORR = mask_table[g_pin_cfg[32].pin & 0xff];
      } else if (pin == 33) {
        port_table[g_pin_cfg[33].pin >>8]->PORR = mask_table[g_pin_cfg[33].pin & 0xff];
      } else if (pin == 34) {
        port_table[g_pin_cfg[34].pin >>8]->PORR = mask_table[g_pin_cfg[34].pin & 0xff];
      } else if (pin == 35) {
        port_table[g_pin_cfg[35].pin >>8]->PORR = mask_table[g_pin_cfg[35].pin & 0xff];
      } else if (pin == 36) {
        port_table[g_pin_cfg[36].pin >>8]->PORR = mask_table[g_pin_cfg[36].pin & 0xff];
      } else if (pin == 37) {
        port_table[g_pin_cfg[37].pin >>8]->PORR = mask_table[g_pin_cfg[37].pin & 0xff];
      } else if (pin == 38) {
        port_table[g_pin_cfg[38].pin >>8]->PORR = mask_table[g_pin_cfg[38].pin & 0xff];
      }
    }
  } else {
    digitalWrite(pin, val);
  }
}
