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

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
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
#define BUFFER_SIZE 80
char buffer[BUFFER_SIZE];
char buffer_Serial[BUFFER_SIZE];
char buffer_Serial1[BUFFER_SIZE];
char buffer_SerialX[BUFFER_SIZE];

uint8_t cb_buffer_Serial = 0;
uint8_t cb_buffer_Serial1 = 0;
uint8_t cb_buffer_SerialX = 0;

uint32_t millis_last_input = 0;

void CopyFromTo(Stream &SerialF, Stream &SerialT, char *buffer, uint8_t &buffer_index) {
  int available;
  int available_for_write;
  int cb;
  if ((available = SerialF.available()) != 0) {
    available_for_write = SerialT.availableForWrite();
    cb = MIN(MIN(available, available_for_write), (int)sizeof(buffer));
    if (cb) {
      SerialF.readBytes(&buffer[buffer_index], cb);
      SerialT.write(&buffer[buffer_index], cb);
      buffer_index += cb;
      millis_last_input = millis();
    }
  }
}

void memory_dump(const char *pb, uint8_t cb) {
  Serial.print("\t");
  for (uint8_t i = 0; i < cb; i++ ) {
    if (*pb < 0x10) Serial.write('0');
    Serial.print(*pb, HEX);
    Serial.print(" ");
    pb++;
  }
  Serial.println();
}


void CompareBuffers(const char *buff1, uint8_t cb1, const char *buff2, uint8_t cb2) {
  if (cb1 == cb2) {
    if (memcmp(buff1, buff2, cb1) == 0) {
      Serial.println("** Match **");
      return;
    } else {
      Serial.println("** different **");
    }
  } else {
    Serial.println("** counts different **");
  }
  memory_dump(buff1, cb1);
  memory_dump(buff2, cb2);
}

void loop() {
  CopyFromTo(Serial, Serial1, buffer_Serial, cb_buffer_Serial);
  CopyFromTo(Serial1, Serial, buffer_Serial1, cb_buffer_Serial1);
  CopyFromTo(SerialX, SerialX, buffer_SerialX, cb_buffer_SerialX);

  // now see if we should compare the data yet or not
  if (cb_buffer_Serial && ((millis() - millis_last_input) > 100)) {
    Serial.println("Check buffers: ");

    Serial.print("  ");
    Serial.println(cb_buffer_Serial, DEC);
    Serial.print("  ");
    Serial.print(cb_buffer_Serial1, DEC);
    Serial.print(" ");
    CompareBuffers(buffer_Serial, cb_buffer_Serial, buffer_Serial1, cb_buffer_Serial1);
    Serial.print("  ");
    Serial.println(cb_buffer_SerialX, DEC);
    CompareBuffers(buffer_Serial, cb_buffer_Serial, buffer_SerialX, cb_buffer_SerialX);

    cb_buffer_Serial = 0;
    cb_buffer_Serial1 = 0;
    cb_buffer_SerialX = 0;
  }
  digitalWrite(5, HIGH);
  delayMicroseconds(100);  // give time form things to propagate
  digitalWrite(5, LOW);
}
