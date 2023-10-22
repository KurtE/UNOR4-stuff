#include <GIGA_digitalWriteFast.h>

void setup() {
  // put your setup code here, to run once:
  while (!Serial && millis() < 5000) {}
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void test_pin(const char *name, pin_size_t pin) {
  Serial.print("Test Pin by number: ");
  Serial.println(name);

  for (uint8_t i = 0; i < 2; i++) {
    digitalWriteFast(pin, HIGH);
    delay(250);
    digitalWriteFast(pin, LOW);
    delay(250);
  }
  for (uint8_t i = 0; i < 4; i++) {
    digitalToggleFast(pin);
    delay(500);
  }
  digitalWriteFast(pin, HIGH);
}

void test_pin(const char *name, PinName pin) {
  Serial.print("Test Pin by name: ");
  Serial.println(name);

  for (uint8_t i = 0; i < 2; i++) {
    digitalWriteFast(pin, HIGH);
    delay(250);
    digitalWriteFast(pin, LOW);
    delay(250);
  }
  for (uint8_t i = 0; i < 4; i++) {
    digitalToggleFast(pin);
    delay(500);
  }
  digitalWriteFast(pin, HIGH);
}


void loop() {
  // put your main code here, to run repeatedly:
  test_pin("LED_BUILTIN", LED_BUILTIN);
  test_pin("LED_RED", LED_RED);
  test_pin("LED_GREEN", LED_GREEN);
  test_pin("LED_BLUE", LED_BLUE);
  test_pin("86", 86);
  test_pin("87", 87);
  test_pin("88", 88);
  test_pin("D86", D86);
  test_pin("D87", D87);
  test_pin("D88", D88);

}
