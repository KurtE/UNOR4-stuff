#include "api/RingBuffer.h"
#include "FspTimer.h"

#define BUF_SIZE 256
RingBufferN<BUF_SIZE> buf;
FspTimer timer;
volatile uint32_t error_count = 0;
volatile uint32_t last_error_count = 0;
uint32_t loop_count = 0;
uint32_t last_report_time = 0;


void setup() {
  while (!Serial && millis() < 5000) 
    ;  // wait for serial...
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // get a timer.
  uint8_t type;
  uint8_t ch = FspTimer::get_available_timer(type);
  timer.begin(TIMER_MODE_PERIODIC, type, ch, 10000.0, 50.0, timerISR, nullptr);
  timer.setup_overflow_irq();
  timer.open();
  timer.start();
}

void timerISR(timer_callback_args_t *arg) {
  // lets did quick validate of buffer
  int avail = buf.available();
  int computed_avail = 0;
  if (buf._iHead >= buf._iTail) computed_avail = buf._iHead - buf._iTail;
  else computed_avail = BUF_SIZE + buf._iHead - buf._iTail;
  if (avail != computed_avail) {
    digitalWrite(LED_BUILTIN, HIGH);
    error_count++;
    buf._numElems = computed_avail;
  }

  buf.store_char(loop_count & 0xff);
}

void loop() {
  loop_count++;

  int ch;
  while ((ch = buf.read_char()) != -1);  // may check some here...

  if (error_count != last_error_count) {
    Serial.print("*");
    Serial.print(error_count, DEC);
    Serial.print(" ");
    Serial.println(loop_count, DEC);
    last_error_count = error_count;
    last_report_time = millis();
  }
  if ((millis() - last_report_time) > 10000) {
    Serial.println(loop_count, DEC);
    last_report_time = millis();
    digitalWrite(LED_BUILTIN, LOW);
  }
}
