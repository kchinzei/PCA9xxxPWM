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
#include <PCA9622PWM.h>
#include <PCA9624PWM.h>
#include <PCA9626PWM.h>
#include <PCA9632PWM.h>
#include <PCA9685PWM.h>
#include <PCA9955APWM.h>
#include <PCA9956APWM.h>
#define DEFAULT_I2C_ADDR_9622 0x72 // Switch Science #2388 mod SJ2
#define DEFAULT_I2C_ADDR_9624 0x60 // Switch Science #2389 default
#define DEFAULT_I2C_ADDR_9626 0x40 // Switch Science #2540 default
#define DEFAULT_I2C_ADDR_9632 0x62 // Switch Science #2378 default
#define DEFAULT_I2C_ADDR_9685 0x40 // Adafruit #815 default
#define DEFAULT_I2C_ADDR_9955 0x65 // Switch Science #2676 default
#define DEFAULT_I2C_ADDR_9956 0x3F // Switch Science #2677 default

// Here you can change which IC to test
#define PCA9XXXPWM PCA9622PWM
#define PCA9XXX_I2C_ADDR DEFAULT_I2C_ADDR_9622

PCA9XXXPWM pwm(PCA9XXX_I2C_ADDR);

uint8_t n_of_ports = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  if (pwm.begin() == false) {
    Serial.println("Fail. Attempt to reset");
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

  test_exponential();
}

void test_exponential() {
  Serial.println("test_exponential() : Compare effect of "
                 "exponential_adjustment() on/off.");
  Serial.println("LED illuminates with the grdation from 0 to 1, Odd LEDs "
                 "exponentially, even LEDs linearly.");
  float step = 1.0 / (n_of_ports / 2);

  for (int j = 0; j < n_of_ports / 2; j++) {
    float ratio = (j + 1) * step;
    pwm.exponential_adjustment(false);
    pwm.pwm(2 * j, ratio);
    pwm.exponential_adjustment(true);
    pwm.pwm(2 * j + 1, ratio);
  }
}

int i = 0;

void loop() {
  pwm.exponential_adjustment(i % 2 == 0); // This dosn't change the brightness.
  delay(1000);
  i++;
}
