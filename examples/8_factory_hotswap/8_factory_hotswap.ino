/*
  The MIT License (MIT)
  Copyright (c) Kiyo Chinzei (kchinzei@gmail.com)
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
/*
  PCA9xxxPWM - Arduino / C++ class for NXP PCA9xxx I2C connected PWM devices.

  Make Asayake to Wake Project.
  Kiyo Chinzei
  https://github.com/kchinzei/PCA9xxxPWM
*/

/*
 Part of code from
 http://radiopench.blog96.fc2.com/blog-entry-520.html
 ラジオペンチ様「I2Cアドレススキャナー (i2cdetectもどき)を作る」
 */
#include "PCA9xxxPWMFactory.h"

#include <Wire.h>
#define MAXDEVICE 4

PCA9xxxPWMFactory factory;
PCA9xxxPWM *pwms[MAXDEVICE];
uint8_t ndevice = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  ndevice = factory.scanDevice(pwms, MAXDEVICE, true);
  delay(100);

  Wire.setClock(400000);
}

void test_one_by_one(PCA9xxxPWM *pwm, int i) {
  int n_of_ports = pwm->number_of_ports();
  for (int j = 0; j < n_of_ports; j++) {
    pwm->pwm(j, (j == i % n_of_ports) ? 1.0 : 0);
  }
}

void initialize_pwm(PCA9xxxPWM *pwm) {
  if (pwm->begin() == false) {
    Serial.println("Fail. Attempt to reset");
    pwm->reset();
    pwm->begin();
  }
}

long counter = 0;

void loop() {
  ndevice = factory.scanDevice(pwms, MAXDEVICE);

  Serial.print("Number of devices = ");
  Serial.println(ndevice);
  Serial.println("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  for (byte n = 0; n <= 0x7F; n++) {
    adrCheck(n);
  }
  Serial.println();
  
  for (int i = 0; i < ndevice; i++) {
    if (!pwms[i]->hasBegun()) {
      initialize_pwm(pwms[i]);
    } else {
      test_one_by_one(pwms[i], counter);
    }
  }
  counter++;
  delay(1000);
}

/*
 * From http://radiopench.blog96.fc2.com/blog-entry-520.html
 */
void adrCheck(byte adr) {
  byte    dummy;
  if ((adr & 0x0F) == 0) {
    print_hex2chr(adr);
    Serial.print(":");
  }
  Serial.print(" ");
 
  if (adr < 8 || adr > 0x77) {
    Serial.print("xx");
  }
  else {
    Wire.beginTransmission(adr);
    Wire.write(&dummy, 0);
    if (Wire.endTransmission() == 0) {
      print_hex2chr(adr);
    }
    else {
      Serial.print("--");
    }
  }
  if ( (adr & 0x0F) == 0x0F) {
    Serial.println();
  }
}

void print_hex2chr(byte x) {
  Serial.print((x >> 4), HEX);
  Serial.print((x & 0x0F), HEX);
}
