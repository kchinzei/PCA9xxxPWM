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
  PCA9xxxPWMFactory - Arduino / C++ class for NXP PCA9xxx I2C connected PWM
  devices.

  Make Asayake to Wake Project.
  Kiyo Chinzei
  https://github.com/kchinzei/PCA9xxxPWM
*/

#include "PCA9xxxPWMFactory.h"
#include "PCA9xxxPWM.h"

#include "PCA9626PWM.h"
#include "PCA9685PWM.h"
#include "PCA9955APWM.h"

static int8_t exists(PCA9xxxPWM** const buf, uint8_t len, uint8_t adr, const String classtype);

PCA9xxxPWMFactory::PCA9xxxPWMFactory(TwoWire *i2cPort) : _i2cPort(i2cPort) {}

PCA9xxxPWMFactory::~PCA9xxxPWMFactory() {}

uint8_t PCA9xxxPWMFactory::scanDevice(PCA9xxxPWM** const oldbuf, uint8_t len, bool reset,
                                      uint8_t i2cAddrFrom, uint8_t i2cAddrTo) {
  PCA9xxxPWM *newbuf[len];
  uint8_t iFound = 0;

  for (int i = 0; i < len; i++) newbuf[i] = NULL;

  // There may be substantial side effects to reset ALL devices.
  if (reset) {
    _i2cPort->begin();
    PCA9xxxPWM::reset(_i2cPort);
    for (int i = 0; i < len; i++) {
      delete oldbuf[i];
      oldbuf[i] = NULL;
    }
  }

  /*
  Serial.print("Before scan");
  for (int i = 0; i < len; i++) {
    Serial.print(" [");
    Serial.print(i);
    Serial.print("]=");
    if (oldbuf[i]) {
        Serial.print(oldbuf[i]->type_name());
        Serial.print(" 0x");
        Serial.print(oldbuf[i]->get_i2cAddr(), HEX);
    } else
        Serial.print("nil");
  }
  Serial.println("");
  */

  if (i2cAddrFrom < 0x08) i2cAddrFrom = 0x08;
  if (i2cAddrTo > 0x7F) i2cAddrTo = 0x7F;

  for (uint8_t i = i2cAddrFrom; i <= i2cAddrTo; i++) {
    if (iFound == len) {
      break;
    }

    // When you add a concrete class of PCA9xxxPWM, add here.

    if (PCA9685PWM::isMyDevice(i, _i2cPort)) {
      int8_t old_index = exists(oldbuf, len, i, PCA9685PWM::class_type());
      if (old_index < 0) {
        newbuf[iFound++] = new PCA9685PWM(i, _i2cPort);
      } else {
        newbuf[iFound++] = oldbuf[old_index];
        oldbuf[old_index] = NULL;
      }
    } else if (PCA9626PWM::isMyDevice(i, _i2cPort)) {
      int8_t old_index = exists(oldbuf, len, i, PCA9626PWM::class_type());
      if (old_index < 0) {
        newbuf[iFound++] = new PCA9626PWM(i, _i2cPort);
      } else {
        newbuf[iFound++] = oldbuf[old_index];
        oldbuf[old_index] = NULL;
      }
    } else if (PCA9955APWM::isMyDevice(i, _i2cPort)) {
      int8_t old_index = exists(oldbuf, len, i, PCA9955APWM::class_type());
      if (old_index < 0) {
        newbuf[iFound++] = new PCA9955APWM(i, _i2cPort);
      } else {
        newbuf[iFound++] = oldbuf[old_index];
        oldbuf[old_index] = NULL;
      }
    }
  }

  for (int i = 0; i < len; i++) {
    delete oldbuf[i];
    oldbuf[i] = newbuf[i];
  }

  /*
  Serial.print("After scan ");
  for (int i = 0; i < len; i++) {
    Serial.print(" [");
    Serial.print(i);
    Serial.print("]=");
    if (oldbuf[i]) {
        Serial.print(oldbuf[i]->type_name());
        Serial.print(" 0x");
        Serial.print(oldbuf[i]->get_i2cAddr(), HEX);
    } else
        Serial.print("nil");
  }
  Serial.println("");
  */

  return iFound;
}

static int8_t exists(PCA9xxxPWM** const buf, uint8_t len, uint8_t adr, const String classtype) {
  for (int i = 0; i < len; i++) {
    if (buf[i]) {
      if (buf[i]->get_i2cAddr() == adr && buf[i]->type_name() == classtype) {
        return i;
      }
    }
  }
  return -1;
}
