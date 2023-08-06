/* USB to Serial - Teensy becomes a USB to Serial converter
   http://dorkbotpdx.org/blog/paul/teensy_as_benito_at_57600_baud

   You must select Serial from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

// set this to the hardware serial port you wish to use
#if defined(ARDUINO_UNOR4_WIFI)
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

#include <SparkFun_Qwiic_Button.h>
QwiicButton button;


UART _UART4_(18, 19);
#define SerialX Serial3
#else
UART _UART2_(18, 19);
#define SerialX Serial2
#endif


#define HWSERIAL SerialX

unsigned long baud = 115200;
const int reset_pin = 4;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(reset_pin, HIGH);
  pinMode(reset_pin, OUTPUT);
  Serial.begin(baud);    // USB, communication to PC or Mac
  HWSERIAL.begin(baud);  // communication to hardware serial

#ifdef ARDUINO_UNOR4_WIFI
  Wire1.setClock(400000);  //set I2C communication to 400kHz
  Wire1.begin();           //Compilation will fail here if your platform doesn't have multiple I2C ports
  if (button.begin(0x6F, Wire1) == false) {
    Serial.println("Button did not begin");
  }
#endif
}

long led_on_time = 0;
byte buffer[80];
unsigned char prev_dtr = 0;

void loop() {
  int rd, wr, n;

  // check if any data has arrived on the USB virtual serial port
  rd = Serial.available();
  if (rd > 0) {
    // check if the hardware serial port is ready to transmit
    wr = HWSERIAL.availableForWrite();
    if (wr > 0) {
      // compute how much data to move, the smallest
      // of rd, wr and the buffer size
      if (rd > wr) rd = wr;
      if (rd > 80) rd = 80;
      // read data from the USB port
      n = Serial.readBytes((char *)buffer, rd);
      // write it to the hardware serial port
      HWSERIAL.write(buffer, n);
      // turn on the LED to indicate activity
      digitalWrite(LED_BUILTIN, HIGH);
      led_on_time = millis();
    }
  }

  // check if any data has arrived on the hardware serial port
  rd = HWSERIAL.available();
  if (rd > 0) {
    // check if the USB virtual serial port is ready to transmit
    wr = Serial.availableForWrite();
    if (wr > 0) {
      // compute how much data to move, the smallest
      // of rd, wr and the buffer size
      if (rd > wr) rd = wr;
      if (rd > 80) rd = 80;
      // read data from the hardware serial port
      n = HWSERIAL.readBytes((char *)buffer, rd);
      // write it to the USB port
      Serial.write(buffer, n);
      // turn on the LED to indicate activity
      digitalWrite(LED_BUILTIN, HIGH);
      led_on_time = millis();
    }
  }

  // check if the USB virtual serial port has raised DTR
#ifndef ARDUINO_UNOR4_WIFI
  unsigned char dtr;
  dtr = Serial.dtr();
  if (dtr && !prev_dtr) {
    digitalWrite(reset_pin, LOW);
    delayMicroseconds(250);
    digitalWrite(reset_pin, HIGH);
  }
  prev_dtr = dtr;
#endif
  // if the LED has been left on without more activity, turn it off
  if (millis() - led_on_time > 3) {
    digitalWrite(LED_BUILTIN, LOW);
  }

#ifdef ARDUINO_UNOR4_WIFI
  static uint8_t matrix_pin = 0;
  static uint32_t matrix_change_time = 0;
  if ((millis() - matrix_change_time) > 250) {
    matrix.off(matrix_pin);
    matrix_pin++;
    if (matrix_pin == 96) matrix_pin = 0;
    matrix.on(matrix_pin);
    matrix_change_time = millis();
    if (button.hasBeenClicked()) {
      button.clearEventBits();
      PrintSerialInfo("\nSerial", Serial);
      PrintSerialInfo("Uart", HWSERIAL);
    }
  }
#else
  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    baud = Serial.baud();
    if (baud == 57600) {
      // This ugly hack is necessary for talking
      // to the arduino bootloader, which actually
      // communicates at 58824 baud (+2.1% error).
      // Teensyduino will configure the UART for
      // the closest baud rate, which is 57143
      // baud (-0.8% error).  Serial communication
      // can tolerate about 2.5% error, so the
      // combined error is too large.  Simply
      // setting the baud rate to the same as
      // arduino's actual baud rate works.
      HWSERIAL.begin(58824);
    } else {
      HWSERIAL.begin(baud);
    }
  }
#endif
}
void PrintSerialInfo(const char *title, UART &uart) {
  R_SCI0_Type *pregs = uart.get_p_reg();
  Serial.println(title);

  uart.printDebugInfo(&Serial);
  Serial.print("\tPregs: ");
  Serial.print((uint32_t)pregs, HEX);
  Serial.print("  SMR: ");
  Serial.print(pregs->SMR, HEX);  //Serial.flush();
  Serial.print("  SCR: ");
  uint8_t scr = pregs->SCR;
  Serial.print(scr, HEX);  //Serial.flush();
  if (scr) {
    Serial.print("(");
    if (pregs->SCR_b.TEIE) { Serial.print(" TEIE"); }
    if (pregs->SCR_b.MPIE) { Serial.print(" MPIE"); }
    if (pregs->SCR_b.RE) { Serial.print(" RE"); }
    if (pregs->SCR_b.TE) { Serial.print(" TE"); }
    if (pregs->SCR_b.RIE) { Serial.print(" RIE"); }
    if (pregs->SCR_b.TIE) { Serial.print(" TIE"); }
    Serial.print(" )");
  }
  Serial.print("  SSR: ");
  uint8_t ssr = pregs->SSR;
  Serial.print(ssr, HEX);  //Serial.flush();
  if (ssr) {
    Serial.print("(");
    if (pregs->SSR_b.MPBT) { Serial.print(" MPBT"); }
    if (pregs->SSR_b.MPB) { Serial.print(" MPB"); }
    if (pregs->SSR_b.TEND) { Serial.print(" TEND"); }
    if (pregs->SSR_b.PER) { Serial.print(" PER"); }
    if (pregs->SSR_b.FER) { Serial.print(" FER"); }
    if (pregs->SSR_b.ORER) { Serial.print(" ORER"); }
    if (pregs->SSR_b.RDRF) { Serial.print(" RDRF"); }
    if (pregs->SSR_b.TDRE) { Serial.print(" TDRE"); }
    Serial.print(" )");
  }
  Serial.print("  SSR_FIFO: ");
  Serial.println(pregs->SSR_FIFO, HEX);  //Serial.flush();
  Serial.print("\tFCR: ");
  Serial.print(pregs->FCR, HEX);  //Serial.flush();
  Serial.print("  FDR: ");
  Serial.print(pregs->FDR, HEX);  //Serial.flush();
  Serial.print("  SPTR: ");
  Serial.println(pregs->SPTR, HEX);  //Serial.flush();
  Serial.print("\tAvail: ");
  Serial.print(uart.available(), DEC);
  Serial.print("  ForWrite: ");
  Serial.println(uart.availableForWrite(), DEC);
}
