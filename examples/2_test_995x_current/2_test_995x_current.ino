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

#include <Wire.h>
#include <PCA9955APWM.h>
#include <PCA9956APWM.h>
#define DEFAULT_I2C_ADDR_9955 0x65 // Switch Science #2676 default
#define DEFAULT_I2C_ADDR_9956 0x3F // Switch Science #2677 default

// Here you can change which IC to test
#define PCA9XXXPWM PCA9956APWM
#define PCA9XXX_I2C_ADDR DEFAULT_I2C_ADDR_9956

PCA9XXXPWM pwm(PCA9XXX_I2C_ADDR);

uint8_t n_of_ports = 0;
boolean verbose = true;

void setup() {
  Serial.begin(115200);
  delay(100);

  if (pwm.begin() == false) {
    pwm.reset();
    if (pwm.begin() == false) {
        Serial.println("Device does not appear to be connected. Please check "
                       "wiring. Freezing...");
        while (1)
            ;
    }
  }

  Wire.setClock(400000);
  n_of_ports = pwm.number_of_ports();
  pwm.exponential_adjustment(true);
}

void test_995x_current(bool current_control_mode) {
  if (verbose) {
    Serial.println(
        "test_995x_current() : Compare effect of current() functions.");
    Serial.println("LED illuminates with the grdation from 0 to 1, Odd LEDs by "
                   "pwm, even LEDs by current.");
    verbose = false;
  }
  pwm.set_current_control_mode(current_control_mode); // See the effect.
  float step = 1.0 / (n_of_ports / 2);
  for (int j = 0; j < n_of_ports / 2; j++) {
    float ratio = (j + 1) * step;
    pwm.pwm(2 * j, 1.0);
    pwm.current(2 * j, ratio);
    pwm.pwm(2 * j + 1, ratio);
    pwm.current(2 * j + 1, 1.0);
  }
}

void test_gradient(bool current_control_mode) {
  float step = 1.0 / n_of_ports;
  pwm.set_current_control_mode(current_control_mode); // See the effect.
  for (int j = 1; j <= n_of_ports; j++) {
    float ratio = step * j;
    pwm.pwm(j - 1, ratio);
    if (verbose) {
      Serial.print("j= ");
      Serial.print(j);
      Serial.print(" ratio= ");
      Serial.println(ratio);
    }
  }
}

int i = 0;

void loop() {
  test_gradient(i % 2 == 0);
  delay(1000);
  i++;
}
