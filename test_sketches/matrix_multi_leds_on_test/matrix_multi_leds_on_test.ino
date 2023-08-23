#include "new_Arduino_LED_Matrix.h"
int matrix_led = -1;
bool matrix_begin_called = false;

ArduinoLEDMatrix matrix;

const uint32_t chip[] = {
  0x1503f811,
  0x3181103,
  0xf8150000
};

const uint32_t danger[] = {
  0x400a015,
  0x1502082,
  0x484047fc
};

const uint32_t happy[] = {
  0x19819,
  0x80000001,
  0x81f8000
};

const uint32_t heart[] = {
  0x3184a444,
  0x44042081,
  0x100a0040
};

const uint32_t all_on[] = {
  0xffffffff,
  0xffffffff,
  0xffffffff
};

const uint32_t *frame_list[] = {
  chip,
  danger,
  happy,
  heart,
  all_on,
};

void setup() {
  // put your setup code here, to run once:
  while (!Serial && millis() < 5000)
    ;
  Serial.begin(115200);
  //  matrix_begin_called = true;
  //  matrix.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    matrix_begin_called = true;
    matrix.begin();
    while (Serial.read() != -1)
      ;
    Serial.println("Matrix begin called");
  }
  
  if (!matrix_begin_called) {

    for (uint8_t led = 0; led < 96; led++) {
      matrix.on(led);
      delay(100);
    }

    for (uint8_t irq_index = 0; irq_index < (sizeof(matrix_irq_pins) / sizeof(matrix_irq_pins[0])); irq_index++) {
      matrix.on(matrix_irq_pins[irq_index][0]);  // first on reset from previous row.
      for (int j = 1; j < 10; j++) {
        if (matrix_irq_pins[irq_index][j] == -1) break;
        matrix.on(matrix_irq_pins[irq_index][j], false);  // first on reset from previous row.
      }
      delay(250);
    }
  } else {
    matrix_led++;
    if (matrix_led >= (int)(sizeof(frame_list) / sizeof(frame_list[0]))) matrix_led = 0;
    matrix.loadFrame(frame_list[matrix_led]);
    delay(1000);
  }
}
