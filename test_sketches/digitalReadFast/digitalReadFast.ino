#include <UNOR4_digitalWriteFast.h>
void setup() {
  // put your setup code here, to run once:
  while (!Serial) {}
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t start_time = micros();
  uint16_t pin2, pin3;
  for (int i = 0; i < 10000; i++) {
    pin2 = digitalRead(2);
  }
  uint32_t delta_time = micros() - start_time;

  start_time = micros();
  for (int i = 0; i < 10000; i++) {
    pin3 = digitalReadFast(3);
  }
  uint32_t delta_time_fast = micros() - start_time;

  Serial.print(delta_time, DEC); Serial.print("\t");
  Serial.print(pin2? 1: 0, DEC); Serial.print("\t");
  Serial.print(delta_time_fast, DEC); Serial.print("\t");
  Serial.println(pin3? 1: 0, DEC); 
  delay(2500);
  


}
