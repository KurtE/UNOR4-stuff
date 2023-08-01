//connect  Serial1 TX -> Serial2 RX, Serial2 TX -> Serial3 RX, Serial3 TX -> Serial4 RX....

#if defined(ARDUINO_UNOR4_WIFI)

UART _UART4_(18, 19);
#define SerialX Serial3
#else
UART _UART2_(18, 19);
#define SerialX Serial2
#endif

#define SPD 115200
int loop_count = 0;

#define BUFFER_SIZE 80

class BufferInfoClass {
public:
  BufferInfoClass() {
    clear();
  }
  char buffer[BUFFER_SIZE];
  uint8_t cb_buffer;
  uint8_t cb_copy[BUFFER_SIZE];
  uint8_t cb_copy_cnt;
  void clear() {
    cb_buffer = 0;
    cb_copy_cnt = 0;
  }
};

BufferInfoClass buffers[3];
uint32_t millis_last_input = 0;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial && millis() < 4000)
    ;
  Serial.begin(115200);
  delay(800);
  Serial.println("Test all Serials");
  Serial.print("Baud rate: ");
  Serial.println(SPD, DEC);
  Serial1.begin(SPD);

  SerialX.begin(SPD);
}

#define MIN(a, b) ((a) <= (b) ? (a) : (b))


void CopyFromTo(Stream &SerialF, Stream &SerialT, uint8_t buffer_index) {
  int available;
  int available_for_write;
  int cb;
  BufferInfoClass *buf = &buffers[buffer_index];
  if ((available = SerialF.available()) != 0) {
    available_for_write = SerialT.availableForWrite();
    cb = MIN(MIN(available, available_for_write), BUFFER_SIZE);
    if (cb) {
      SerialF.readBytes(&buf->buffer[buf->cb_buffer], cb);
      SerialT.write(&buf->buffer[buf->cb_buffer], cb);
      buf->cb_buffer += cb;
      buf->cb_copy[buf->cb_copy_cnt] = cb;
      buf->cb_copy_cnt++;
      millis_last_input = millis();
    }
  }
}

void memory_dump(const char *pb, uint8_t cb) {
  const char* pbA = pb;
  uint8_t cbA = cb;

  Serial.print("\t");
  for (uint8_t i = 0; i < cb; i++) {
    if (*pb < 0x10) Serial.write('0');
    Serial.print(*pb, HEX);
    Serial.print(" ");
    pb++;
  }
  
  Serial.print("\n\t ");
  for (uint8_t i = 0; i < cbA; i++) {
    if (*pbA >= ' ')  Serial.write(*pbA);
    else Serial.write(' ');
    Serial.print("  ");
    pbA++;
  }
  Serial.println();
}

void print_buffer_header(uint8_t index) {
  BufferInfoClass *buf = &buffers[index];
  Serial.print("  ");
  Serial.print(buf->cb_buffer, DEC);
  Serial.print(" (");
  for (uint8_t i = 0; i < buf->cb_copy_cnt; i++) {
    if (i != 0) Serial.print(",");
    Serial.print(buf->cb_copy[i], DEC);
  }
  Serial.print(")");
}

void CompareBuffers(uint8_t index1, uint8_t index2) {
  if (buffers[index1].cb_buffer == buffers[index2].cb_buffer) {
    if (memcmp(buffers[index1].buffer, buffers[index2].buffer, buffers[index1].cb_buffer) == 0) {
      Serial.println(" ** Match **");
      return;
    } else {
      Serial.println(" ** different **");
    }
  } else {
    Serial.println(" ** counts different **");
  }
  memory_dump(buffers[index1].buffer, buffers[index1].cb_buffer);
  memory_dump(buffers[index2].buffer, buffers[index2].cb_buffer);
}

void loop() {
  CopyFromTo(Serial, Serial1, 0);
  CopyFromTo(Serial1, Serial, 1);
  CopyFromTo(SerialX, SerialX, 2);

  // now see if we should compare the data yet or not
  if (buffers[0].cb_buffer && ((millis() - millis_last_input) > 100)) {
    Serial.println("Check buffers: ");

    print_buffer_header(0);
    Serial.println();
    print_buffer_header(1);
    CompareBuffers(0, 1);

    print_buffer_header(2);
    CompareBuffers(0, 2);

    buffers[0].clear();
    buffers[1].clear();
    buffers[2].clear();
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(100);  // give time form things to propagate
  digitalWrite(LED_BUILTIN, LOW);
}
