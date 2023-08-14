/*
  LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");  // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN;  // pin to use for the LED

////////////////////////////////////////////////////////////////////
// Matrix stuff
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

const uint32_t chip[] = {
  0x1503f811,
  0x3181103,
  0xf8150000
};

const uint32_t danger[] = {
  0x400a015,
  0x1502082,
  0x484047fc
};

const uint32_t happy[] = {
  0x19819,
  0x80000001,
  0x81f8000
};

const uint32_t heart[] = {
  0x3184a444,
  0x44042081,
  0x100a0040
};
const uint32_t *frame_list[] = {
  chip, danger, happy, heart
};

int matrix_led = -1;
bool matrix_begin_called = false;
uint32_t led_turned_on_start_time = 0;

////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1)
      ;
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {  // any value other than 0
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);  // will turn the LED on
          led_turned_on_start_time = millis();
        } else {  // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);  // will turn the LED off
          if (!matrix_begin_called && ((millis() - led_turned_on_start_time) > 5000)) {
            // button pressed more than 5 seconds.
            Serial.println("\n***Call matrix.begin() ***");
            matrix.begin();
            matrix_begin_called = true;
            matrix_led = 0;
          }
        }
        if (matrix_begin_called) {
          matrix_led++;
          if (matrix_led >= (int)(sizeof(frame_list) / sizeof(frame_list[0]))) matrix_led = 0;
          matrix.loadFrame(frame_list[matrix_led]);
        } else {
          matrix_led = (matrix_led == 95) ? 0 : matrix_led + 1;
          matrix.on(matrix_led);
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
