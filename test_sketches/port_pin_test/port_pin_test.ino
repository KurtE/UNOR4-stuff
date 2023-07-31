#define GPIO_BITBAND_ADDR(reg, bit) (((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)
volatile uint32_t *p_led_bit = (volatile uint32_t *)GPIO_BITBAND_ADDR(R_PORT1->PODR, 11);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

  while (!Serial && millis() < 5000) ;
  Serial.begin(115200);
  Serial.println((uint32_t)&R_PORT1->PODR, HEX);
  Serial.println((uint32_t)p_led_bit, HEX);
}


uint8_t counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  counter++;
#if 0
  uint8_t portb_val = PINB;
  if (counter & 1) portb_val |= (1 << 5);
  else portb_val &= ~(1 << 5);
  PORTB = portb_val;
#elif 0
  uint16_t  d1 = R_PORT1->PODR;
  if (counter & 1) d1 |= (1 << 11);
  else d1 &= ~(1 << 11);
  R_PORT1->PODR = d1;
#else
  if (counter & 1) *p_led_bit = 1;
  else  *p_led_bit = 0;

#endif  
  delay(250);
}
