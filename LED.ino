/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

/*
 * Sketch: led.ino
 *
 * Description:
 *   This is a Peripheral sketch that works with a connected Central.
 *   It allows the Central to write a value and set/reset the led
 *   accordingly.
 */

#include <CurieBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLEWrite);

const int ledPin1 = 12; // pin to use for the LED1
const int ledPin2 = 11; // pin to use for the LED2

void setup() {
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // begin initialization
  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
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
        if (switchCharacteristic.value() == 1) {   // any value other than 0
          Serial.println("LED 1 on");
          digitalWrite(ledPin1, HIGH);         // will turn the LED on
        } else if(switchCharacteristic.value() == 2) {
          Serial.println("LED 2 on");
          digitalWrite(ledPin2, HIGH);
        } else if (switchCharacteristic.value() == 3) {
          Serial.println("LED 1 off");
          digitalWrite(ledPin1, LOW);
        } else if (switchCharacteristic.value() == 4) {
          Serial.println("LED 2 off");
          digitalWrite(ledPin2, LOW);
        } else {
          Serial.println(F("LEDs off"));
          digitalWrite(ledPin1, LOW);
          digitalWrite(ledPin2, LOW);
        }       
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
