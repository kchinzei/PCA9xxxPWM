# PCA9xxxPWM
Arduino / C++ class for NXP PCA9xxx I2C connected PWM devices

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![PCA9955A example](https://d2air1d4eqhwg2.cloudfront.net/images/2676/500x500/b2f37069-69b3-4af2-acb9-204be4aad3fb.jpg)

## About PCA9xxx Series PWM Devices

PCA9xxxPWM is an Arduino library to control PWM outputs of NXP PCA962x, [PCA9685](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-channel-12-bit-pwm-fm-plus-ic-bus-led-controller:PCA9685), PCA995xA I2C bus connected devices.
PCA962x series are
[PCA9622](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9622) (16ch),
[PCA9624](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9624) (8ch),
[PCA9626](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9626) (24ch).
'B' can be suffixed.
PCA995xA series are
[PCA9955A](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-channel-fm-plus-ic-bus-57-ma-20-v-constant-current-led-driver:PCA9955BTW) (16ch),
[PCA9956A](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/24-channel-fm-plus-ic-bus-57-ma-20-v-constant-current-led-driver:PCA9956BTW) (24ch).
'B*' can be suffixed instead of 'A'. PCA995x (no suffix) are incompatible.

You can find breakout boards from Switch Science,
[PCA9622](https://international.switch-science.com/catalog/2388/),
[PCA9955BTW](https://international.switch-science.com/catalog/2676/) etc.

There are the official mbed libraries for these devices from NXP (ex: [PCA962x](https://os.mbed.com/users/nxp_ip/code/PCA962x/), [PCA995xA](https://os.mbed.com/users/nxp_ip/code/PCA995xA/)), but it seems none for Arduino. PCA9xxxPWM is here for you.

These devices have different features in number of outputs, allowable current / voltage, bit depth of PWM, current vs. voltage switch PWM, fading out control, etc. But all they control PWM ratio in common.

## Using PCA9xxxPWM

Currently PCA9626 and PCA9955A are implemented and tested. Popular PCA9685 will be implemented soon.

Copy `PCA9xxxPWM` folder into 'libraries' folder of Arduino, where Arduino IDE can search for files.
You can find some examples in `examples` folder.

As other I2C connection for Arduino does, PCA9xxxPWM uses `TwoWire` for I2C. By default `Wire` is used. I2C address is in 7 bits, without the last R/W bit.

You instantiate a concrete class of Class `PCA9xxxPWM`, for example `PCA9626PWM` object by specifying its I2C address.

```C++
#include <Arduino.h>
#include <Wire.h>
#include "PCA9955APWM.h"
PCA9955APWM pwmdevice(0x65);
```
Or you can use `PCA9xxxPWMFactory` class to scan I2C bus and spawn instances for connected PCA9xxx devices.

```C++
#include "PCA9xxxPWMFactory.h"
PCA9xxxPWM* pwmdevices[4];
PCA9xxxPWMFactory factory;
uint8_t ndevice = factory.scanDevice(pwmdevices, 4);
```

Before using a `PCA9xxxPWM` instance, you need to call `begin()` to initialize it.
You can find the device's maximum PWM outputs using `number_of_ports()`.

```C++
if (pwmdevice.begin()) {
  // Device initialized and ready to use.
  uint8_t n = pwmdevice.number_of_ports();
  ...
```

PCA9xxxPWM controls PWM ratio of each output.

```C++
pwmdevice.pwm(ch, 0.78);
```

`pwm()` accepts floating number between 0 and 1 to represent the ratio.
You can set it one by one. Alternatively you can send an array of values for ALL PWM outputs.
```C++
float vals[16] = {0.1, 0.2, ... 1};
pwmdevice.pwm(vals);
```

### Static Class Member functions

There are a few static class member functions so that you can examine I2C devices before instantiating one. Calling them will force the `TwoWire` object initialized by internally calling `begin()`, which should be no harm.

```C++
static boolean isConnected(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);
static boolean isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);
```

`isMyDevice()` is available for every concrete class under `PCA9xxxPWM`. They assume that the device is in the default state of each device. See each implementation to learn what is assumed.

### Hardware dependent member functions

You need to set current gain for PCA995xA devices. Note that there aren't equivalent member functions for other devices.

```C++
void current(uint8_t port, float vp);
void current(float *vP);
```

PCA9955A has a feature to exponentially enhance the output so that the brightness change felt naturally. Use `exponential_adjustment()` to turn on/off it.

```C++
void exponential_adjustment(boolean exp_on);
```

For other devices, it is implemented by software. They behave differently from PCA9955A when turning on/off during controlling PWM outputs.

## To Add More Devices

You can add other device by deriving a concrete class from `PCA9xxxPWM` or its subclass. When doing this, you also need to

- Provide your own `isMyDevice()` and call it from `PCA9xxxPWM::isMyDevice()` or subclass's `isMyDevice()`.
- Modify `PCA9xxxPWMFactory::scanDevice()` to spawn your instance.

# License

The MIT License (MIT) Copyright (c) K. Chinzei (kchinzei@gmail.com) Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
