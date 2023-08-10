void setup() {
  while (!Serial) ;
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

uint8_t loop_count = 0;
uint32_t last_baud = 0;
void loop() {
  digitalWrite(LED_BUILTIN, (++loop_count & 1)? HIGH : LOW);

  if (last_baud != Serial.baud()) {
    last_baud = Serial.baud();
    Serial.print("New Baud:");
    Serial.println(last_baud, DEC);
  }

  delay(500);
}
