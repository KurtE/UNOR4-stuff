/*
  LED Control

  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <SparkFun_Qwiic_Button.h>
QwiicButton button;
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;
#include "frames.h"

// variables for button
const int buttonPin = 2;
bool oldButtonState = false;

int matrix_led = -1;
bool matrix_begin_called = false;
uint32_t button_pressed_start_time = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // configure the button pin as input
  //pinMode(buttonPin, INPUT);
  Wire1.begin();
  if (!button.begin(0x6F, Wire1)) {
    Serial.print("*** Button did not start ***");
  }


  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LED") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    // read the button pin
    bool buttonState = button.isPressed();
    //int buttonState = digitalRead(buttonPin);

    if (oldButtonState != buttonState) {
      // button changed
      oldButtonState = buttonState;

      if (buttonState) {
        Serial.println("button pressed");

        // button is pressed, write 0x01 to turn the LED on
        ledCharacteristic.writeValue((byte)0x01);
        button_pressed_start_time = millis();
      } else {
        Serial.println("button released");

        // button is released, write 0x00 to turn the LED off
        ledCharacteristic.writeValue((byte)0x00);
      }
      if (!matrix_begin_called && ((millis() - button_pressed_start_time) > 5000)) {
        // button pressed more than 5 seconds.
        Serial.println("\n***Call matrix.begin() ***");
        matrix.begin();
        matrix_begin_called = true;
        matrix_led = 0;
      }
      if (matrix_begin_called) {
        matrix_led++;
        if (matrix_led >= (int)(sizeof(frame_list)/ sizeof(frame_list[0]))) matrix_led = 0;
        matrix.loadFrame(frame_list[matrix_led]);
      } else {
        matrix_led = (matrix_led == 95)? 0 : matrix_led + 1;
        matrix.on(matrix_led);
      }      
    }
  }

  Serial.println("Peripheral disconnected");
}
