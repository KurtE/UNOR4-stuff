static inline void digitalWriteFast(uint8_t pin, uint8_t val) __attribute__((always_inline, unused));
//#include "digitalFast.h"
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
static inline void digitalWriteFast(pin_size_t pin, PinStatus val) {
  uint16_t hardware_port_pin = g_pin_cfg[pin].pin;
  //uint16_t mask = 1 << (hardware_port_pin & 0xf);
  uint16_t mask = mask_table[hardware_port_pin & 0xf];
  R_PORT0_Type *port = port_table[hardware_port_pin >> 8];
  if (val) port->POSR = mask;
  else port->PORR = mask;
}
