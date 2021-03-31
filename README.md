# PCA9xxxPWM
Arduino / C++ class for NXP PCA9xxx I2C connected PWM devices

## ABout PCA9xxxPWM

PCA9xxxPWM is an Arduino library to control PWM outputs of NXP PCA9622, 9624, 9626, 9685, 9955A, 9956B I2C bus connected devices.
There are the official mbed libraries for these devices (ex: [PCA962x](https://os.mbed.com/users/nxp_ip/code/PCA962x/), [PCA995xA](https://os.mbed.com/users/nxp_ip/code/PCA995xA/)), but it seems none for Arduino. Here it is.

These devices have different features in number of outputs, allowable current / voltage, bit depth of PWM, current vs. voltage switch PWM, fading out control, etc. But in common all they control PWM ratio.

## Using PCA9xxxPWM

Copy `PCA9xxxPWM` folder into 'libraries' folder of Arduino, where Arduino IDE can search for files.
You can find some examples in `examples` folder.

You instantiate a concrete class of Class `PCA9xxxPWM`, for example `PCA9626PWM` object by specifying its I2C address.

```C++
#include <PCA9955APWM.h>
#define DEFAULT_I2C_ADDR 0x65
PCA9955APWM pwm(DEFAULT_I2C_ADDR);
```
Or you can use `PCA9xxxPWMFactory` class to scan I2C bus and spawn instances for connected PCA9xxx devices.

```C++
#include "PCA9xxxPWMFactory.h"
#define MAXDEVICE 4

PCA9xxxPWM* pwms[MAXDEVICE];
uint8_t ndevice = 0;
PCA9xxxPWMFactory factory;
ndevice = factory.scanDevice(pwms, MAXDEVICE);
```

Before using an instance, you need to call `begin()` to take care of I2C communication.
You can find the device's maximum PWM outputs using `number_of_ports()`.

```C++
if (pwm.begin()) {
  int n = pwm.number_of_ports();
  ...
```

PCA9xxxPWM controls PWM ratio of each output.

```C++
pwm.pwm(ch, 0.78);
```

`pwm()` accepts folating number between 0 and 1 to represent the ratio.
You can set it one by one. Alternatively you can send an array of values for ALL PWM outputs.

## Hardware dependent member functions

You can set current gain for PCA995xA devices.

```C++
void current(uint8_t port, float vp);
void current(float *vP);
```

#License

The MIT License (MIT) Copyright (c) K. Chinzei (kchinzei@gmail.com) Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
