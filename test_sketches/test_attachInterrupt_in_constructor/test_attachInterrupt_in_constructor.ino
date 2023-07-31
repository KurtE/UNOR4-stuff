#define USE_ONESTEP_CONSTRUCTOR

class allocate_something {
public:
  allocate_something() {
    time_created = micros();
  }
  uint32_t time_created;
  uint32_t time_set;
  char buffer[40];
};

class test_stuff_in_constructor {
public:
  void begin(uint8_t pin) {
    if (_pin != 0xff) {
      Serial.println("begin: was previously called");
      return;
    }
    _pin = pin;
    pas = new allocate_something;
    if (pas) {
      pas->time_set = micros();
      strcpy(pas->buffer, "abcdefghijklmnop");
    }
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, &changeFunc, CHANGE);
    pinMode(LED_BUILTIN, OUTPUT);
    _counter = 0;
  }

  test_stuff_in_constructor() {}

  test_stuff_in_constructor(uint8_t pin) {
    begin(pin);
  }
  allocate_something *pas = nullptr;

  uint32_t count() {
    return _counter;
  }
  uint8_t pin() {
    return _pin;
  }

private:
  uint8_t _pin = 0xff;

  static volatile uint32_t _counter;
  static void changeFunc() {
    digitalWrite(LED_BUILTIN, HIGH);
    _counter++;
  }
};

#ifdef USE_ONESTEP_CONSTRUCTOR
test_stuff_in_constructor testobj(2);
#else
test_stuff_in_constructor testobj;
#endif
volatile uint32_t test_stuff_in_constructor::_counter = 0;

extern void DumpIRQTable();
char print_buffer[80];
extern void Check_attach_interrupt(uint8_t, uint8_t);  ////////////////////

void setup() {
  while (!Serial && millis() < 4000)
    ;
  Serial.begin(115200);
  Serial.println("Test constructor stuff");
  Check_attach_interrupt(2, 0xff);  ////////////////////

  DumpIRQTable();

#ifndef USE_ONESTEP_CONSTRUCTOR
  testobj.begin(2);
  DumpIRQTable();
#endif

  sprintf(print_buffer, "PAS: %p %u %u %s\n", testobj.pas, testobj.pas->time_created, testobj.pas->time_set, testobj.pas->buffer);
  Serial.print(print_buffer);


  Serial.print("Pin: ");
  Serial.println(testobj.pin());
}

uint32_t previous_count = 0;
void loop() {
  // put your main code here, to run repeatedly
  uint32_t count = testobj.count();
  if (count != previous_count) {
    Serial.println(count, DEC);
    previous_count = count;
  }
}
