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

#include "PCA9624PWM.h"

PCA9624PWM::PCA9624PWM(uint8_t i2cAddr, TwoWire *i2cPort)
    : PCA962xPWM(i2cAddr, i2cPort), n_of_ports(8) {}

PCA9624PWM::~PCA9624PWM() {}

void PCA9624PWM::init() {
  uint8_t init_array0[] = {
      PCA962xPWM::AUTO_INCREMENT | REGISTER_START, //  Command
      0x01, 0x05,                                  //  MODE1, MODE2
  };
  uint8_t init_array1[] = {
      PCA962xPWM::AUTO_INCREMENT | LEDOUT_REGISTER_START, //  Command
      0xAA,
      0xAA, //  LEDOUT[1:0]
  };

  write(init_array0, sizeof(init_array0));
  write(init_array1, sizeof(init_array1));
}

#define CUSTOM_SUBADR3_VAL 0xEA /* Default is 0xE8 */
#define SUBADR_MASK 0xFE /* bit 0 is reserved so we assume it can be changed */

boolean PCA9624PWM::hasBegun() {
    return isConnected() && ((read(PCA9624PWM::SUBADR3) & SUBADR_MASK) == (CUSTOM_SUBADR3_VAL & SUBADR_MASK));
}

void PCA9624PWM::customHasBegun() {
  uint8_t adr_array[] = {
      PCA9624PWM::SUBADR3,
      CUSTOM_SUBADR3_VAL & SUBADR_MASK
  };
  write(adr_array, sizeof(adr_array));
}

uint8_t PCA9624PWM::pwm_register_access(uint8_t port) {
  if (port < n_of_ports)
    return PWM_REGISTER_START + port;
  else
    return PWMALL;
}

uint8_t PCA9624PWM::number_of_ports() const { return n_of_ports; }

String PCA9624PWM::type_name() const { return PCA9624PWM::class_type(); }

// static
boolean PCA9624PWM::isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort) {
  return PCA962xPWM::isMyDevice(i2cAddr, i2cPort) &&
         PCA9624PWM::_isMyDevice(i2cAddr, i2cPort);
}

// static
boolean PCA9624PWM::_isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort) {
  // PCA9624 returns 0b11100010, 100, 1000 for SUBADR1 - SUBADR3 by default
  uint8_t subadr1 = PCA9xxxPWM::read(i2cAddr, i2cPort, PCA9624PWM::SUBADR1);
  uint8_t subadr2 = PCA9xxxPWM::read(i2cAddr, i2cPort, PCA9624PWM::SUBADR2);

  return subadr1 == 0b11100010 && subadr2 == 0b11100100;
}
