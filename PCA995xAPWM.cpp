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

#include "PCA995xAPWM.h"

PCA995xAPWM::PCA995xAPWM(uint8_t i2cAddr, TwoWire *i2cPort)
    : PCA9xxxPWM(i2cAddr, i2cPort) {}

PCA995xAPWM::~PCA995xAPWM() {}

boolean PCA995xAPWM::begin() {
  if (PCA9xxxPWM::isConnected(_i2cAddr, _i2cPort) &&
      PCA995xAPWM::isMyDevice(_i2cAddr, _i2cPort)) {
    return PCA9xxxPWM::begin();
  } else {
    return false;
  }
}

void PCA995xAPWM::reset(void) {
  PCA995xAPWM::reset(_i2cPort);
}

// static
void PCA995xAPWM::reset(TwoWire *i2cPort) {
  uint8_t va[] = {0x06};
  write(ADR_RESET, i2cPort, va, sizeof(va));
}

// Current limit and PWM

void PCA995xAPWM::set_current_control_mode(bool mode) {
  use_current_control = mode;
}

#define SMALL_V 16

void PCA995xAPWM::pwm(uint8_t port, float v) {
  if (use_current_control) {
    PCA9xxxPWM::pwm(port, v);
  } else {
    v = simple_exp(v);
    uint8_t vc = v * 255;
    uint8_t vp = 255;
    if (vc < SMALL_V) {
      // When small we represent fractional part using _pwm()
      vc++;
      vp = v / vc * 65535;
    }
    _pwm(port, vp);
    _current(port, vc);
  }
}

void PCA995xAPWM::pwm(float *vp) {
  uint8_t n_of_ports = number_of_ports();
  uint8_t data_c[n_of_ports];
  uint8_t data_p[n_of_ports];

  for (int i = 1; i <= n_of_ports; i++) {
    float v = simple_exp(*vp++)*255;
    data_c[i] = v * 255;
    data_p[i] = 255;
    if (data_c[i] < SMALL_V) {
      data_c[i]++;
      data_p[i] = v / data_c[i] * 65535;
    }
  }
  _pwm(data_p);
  _current(data_c);
}

void PCA995xAPWM::current(uint8_t port, float v) {
  _current(port, (uint8_t)(v * 255));
}

void PCA995xAPWM::current(float *vp) {
  uint8_t n_of_ports = number_of_ports();
  uint8_t data[n_of_ports];

  for (int i = 1; i <= n_of_ports; i++) {
    data[i] = (uint8_t)(*vp++ * 255);
  }
  _current(data);
}

float PCA995xAPWM::get_current(uint8_t port) {
  uint8_t reg_addr = current_register_access(port);
  return read(reg_addr) / 255.0;
}

void PCA995xAPWM::_current(uint8_t port, uint8_t v) {
  uint8_t reg_addr = current_register_access(port);
  write(reg_addr, v);
}

void PCA995xAPWM::_current(uint8_t *vp) {
  uint8_t n_of_ports = number_of_ports();
  uint8_t data[n_of_ports + 1];

  *data = current_register_access(0) | AUTO_INCREMENT;

  for (int i = 1; i <= n_of_ports; i++) {
    data[i] = *vp++;
  }
  write(data, sizeof(data));
}

// static
boolean PCA995xAPWM::isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort) {
  if (i2cAddr == ADR_ALLCALL || i2cAddr == ADR_SUBADR_DEFAULT)
    return false;
  // PCA995x devices commonly return 0b10001001 for MODE1, 0b00000*01 for MODE 2
  // by default. Bit 2 of MODE2 differs between 9955 and 9956.
  uint8_t mode1 = PCA9xxxPWM::read(i2cAddr, i2cPort, PCA9xxxPWM::MODE1);
  uint8_t mode2 = PCA9xxxPWM::read(i2cAddr, i2cPort, PCA9xxxPWM::MODE2);

  /*
  if (mode2) {
    Serial.print(" Adr = 0x");
    Serial.print(i2cAddr, HEX);
    Serial.print(" MODE1 = ");
    Serial.print(mode1, BIN);
    Serial.print(" MODE2 = ");
    Serial.println(mode2, BIN);
  }
  */

  // However, experiments reveal that
  // 1) MODE1 bit 7 (auto increment) can be 0,
  // 2) MODE1 bit 4 (sleep) can be 0,
  // 3) MODE1 bit 0 (allcall) can be 0,
  // 4) MODE2 bit 6 (open circuit) can be 1.
  // 4) MODE2 bit 2 (exponential) can be 0.
  // return mode1 == 0b00001001 && (mode2 | 1 << 2) == 0b00000101;
  return (mode1 & 0b01110110) == 0b00000000 &&
         (mode2 & 0b10111011) == 0b00000001;
}
