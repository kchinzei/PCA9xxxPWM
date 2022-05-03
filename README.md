# PCA9xxxPWM
Arduino/PlatformIO C++ class for NXP PCA9xxx I2C connected PWM devices

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![PCA9955A example](https://d2air1d4eqhwg2.cloudfront.net/images/2676/500x500/b2f37069-69b3-4af2-acb9-204be4aad3fb.jpg)

## About PCA9xxx Series PWM Devices

PCA9xxxPWM is an Arduino/PlatformIO library to control PWM outputs of NXP PCA962x, [PCA9685](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-channel-12-bit-pwm-fm-plus-ic-bus-led-controller:PCA9685), PCA995xA I2C bus connected devices.
PCA962x series are 8 bit PWM controllers, with
[PCA9622](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9622) (16ch),
[PCA9624](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9624) (8ch),
[PCA9626](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-bit-fm-plus-ic-bus-100-ma-40-v-led-driver:PCA9626) (24ch),
[PCA9632](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/4-bit-fm-plus-ic-bus-low-power-led-driver:PCA9632) (4ch).
'B' can be suffixed.
[PCA9685](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-channel-12-bit-pwm-fm-plus-ic-bus-led-controller:PCA9685) is a 16ch 12bit PWM controller.
PCA995xA series are 8 bit PWM controllers, with
[PCA9955A](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-channel-fm-plus-ic-bus-57-ma-20-v-constant-current-led-driver:PCA9955BTW) (16ch),
[PCA9956A](https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/24-channel-fm-plus-ic-bus-57-ma-20-v-constant-current-led-driver:PCA9956BTW) (24ch).
'B*' can be suffixed instead of 'A'. PCA995x (no suffix) are incompatible.

You can find breakout boards from [Switch Science](https://international.switch-science.com/),
[PCA9622](https://international.switch-science.com/catalog/2388/),[PCA9624](https://international.switch-science.com/catalog/2389/), [PCA9626](https://international.switch-science.com/catalog/2540/),
[PCA9955BTW](https://www.switch-science.com/catalog/2676/), [PCA9956BTW](https://www.switch-science.com/catalog/2677/) etc.
PCA9685 breakout boards are available from [Adafruit](https://www.adafruit.com/product/815), [Sparkfun](https://www.sparkfun.com/products/15316) and Switch Science.

There are NXP official mbed libraries for [PCA962x](https://os.mbed.com/users/nxp_ip/code/PCA962x/) and [PCA995xA](https://os.mbed.com/users/nxp_ip/code/PCA995xA/), but it seems none for Arduino or PlatformIO.
Adafruit published [PWM Servo Driver Library for PCA9685](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) for Arduino.
It is not compatible and applicable to PCA962x, 995xA due to substantial differences from PCA9685. PCA9xxxPWM is here for you.

These devices have different features in number of outputs, allowable current / voltage, bit depth of PWM, current vs. voltage switch PWM, fading out control, etc. But all they control PWM ratio in common.

## Using PCA9xxxPWM

Currently PCA9622, PCA9624, PCA9626, PCA9632, PCA9685, PCA9955A and PCA9956A are implemented and tested. Here it is mainly explained for Arduino. Basically same for PlatformIO. See [PlatformIO](#platformio).

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
You can also call it in the main loop to examine if a device is (un)pluged. See [Response to device connection / removal](#Response-to-device-connection-removal)

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

The core function of `PCA9xxxPWM` is `pwm()` to control PWM ratio of each output.

```C++
pwmdevice.pwm(ch, 0.78);
```

`pwm()` accepts floating number between 0 and 1 to represent the ratio.
You can set it one by one. Alternatively you can send an array of values for ALL PWM outputs.
```C++
// Size of array must be at least the number of ports.
float vals[16] = {0.1, 0.2, ... 1};
pwmdevice.pwm(vals);
```

### PlatformIO

It also partly tested for [PlatformIO](https://platformio.org) running on [Visual Studio Code](https://code.visualstudio.com).
When you use it in a PlatformIO project, basically you can put PCA9xxxPWM folder wherever appropriate (or wherever you like). Default place is `./lib` under the project's root folder.
But you need to inform where you put it.
You can use `lib_dir` in `platformio.ini`.
For example, if you put PCA9xxxPWM folder in `wled00/extLibs` under the project's root folder:

```PlatformIO
[platformio]
default_envs = esp32dev

lib_dir = ./wled00/extLibs
...
```

This puts `./wled00/extLibs/PCA9xxxPWM` (and any folder in `extLibs`) in the include and source file search.
So all you need in your code is to include them like:

```C++
#include <PCA9xxxPWM.h>
#include <PCA9626.h>
```

See an example in [WLED-PCA9xxx](https://github.com/kchinzei/WLED-PCA9xxx.git).

### Static class member functions

There are a few static class member functions so that you can examine I2C devices before instantiating one. Calling them will force the `TwoWire` object initialized by internally calling `begin()`, which should be no harm.

```C++
static boolean isConnected(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);
static boolean isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);
```

`isMyDevice()` is available for every concrete class under `PCA9xxxPWM`. They assume that the device is in the default state of each device. See each implementation to learn what is assumed.

### Response to device connection / removal

You may want hot swap of I2C connection.
You can watch if the device is not initialized by `begin()` by periodically calling `hasBegun()`.
Code snippet is found in `examples/7_hasBegun`.
For the same purpose, you can safely call `PCA9xxxPWMFactory::scanDevice()` periodically in the main loop.
Doing so correctly preserves the status of existing `PCA9xxxPWM` instances as far as the device associated to the instance is connected.
Code snippet is found in `examples/8_factory_hotswap`.

`hasBegun()` modifies and watches `SUBADR3`.
If it goes back to the default, it means the device was not initialized yet.
If you use `SUBADR3` and need to change it for your purpose,
you need to implement your own `hasBegun()` and `customHasBegun()`
by deriving from a concrete class of `PCA9xxxPWM`.

## Hardware dependent member functions and behaviors

### PCA995xA series specific functions

You can use current control for PCA995xA devices. Note that there aren't equivalent member functions for other devices.

```C++
void PCA995xAPWM::current(uint8_t port, float vp);
void PCA995xAPWM::current(float *vP);
```

By default `pwm()` of PCA995xAPWM uses current control for PCA995xA devices. It also uses PWM control for finer control when value is small. You can take full control of both `pwm()` and `current()` by turning on `set_current_control_mode()`.

```C++
void PCA995xAPWM::set_current_control_mode(bool mode)
```

PCA995xA devices can also detect open/short circuit of output ports and over temperature condition.
`errflag()` returns error conditions.
Note that calling this function clears the flag, however it does not solve the physical fault. When open/short circuit happens you must turn off such ports for safety.

```C++
uint8_t PCA995xAPWM::errflag(uint8_t port);
```

When current output is small, it can raise false short circuit error. It's because the error detection is done by examining the effective output voltage exceeds ca. 2.85V. When the current is small, output voltage can exceed this threshold.

PCA9955A has a feature to exponentially change the gradation output so that the brightness change felt naturally for human eyes.
But this feature only works for gradient control.
Therefore, `PCA9xxxPWM` does it by software for all PCA9xxx devices, for `pwm()` control.
Use `exponential_adjustment()` to turn on/off it.

```C++
void PCA9xxxPWM::exponential_adjustment(boolean exp_on);
```

### PCA9685 specific functions

PCA9685 can change PWM frequency. `PCA9685PWM` has `set_freq()` for this purpose.
It also accepts an external hardware clock. Other devices has fixed, internal oscillators only. To notify the software, provide `ext_clock`.

```C++
void  PCA9685PWM::freq(float freq, float ext_clock = 0.0);
float PCA9685PWM::calc_freq(float ext_clock = 0.0);
```

PCA9685 and PCA9632 can switch its outputs between open drain and totem pole. Other devices are open drain outputs. PCA9685 and PCA9632 can also invert its outputs.

```C++
void PCA9685PWM::totem_pole_outputs(boolean totem_pole);
void PCA9685PWM::invert_outputs(boolean invert);
```

By default it is in **totem pole mode, not inverted** after initialization.
PCA9xxx changes `PCA9685PWM` by calling `begin()` to open-drain, inverted so as it works same as other devices.
Currently these functions are not implemented for `PCA9632PWM`.

### Functions that need some caution

`reset()` broadcast a sequence of command to the I2C bus.

```C++
static void reset(TwoWire *i2cPort); // Class method
void reset(void); // Object method
```

Indeed these two `reset()` do same; both broadcast. The object method version does also broadcast to reset ALL devices of the same class.
Even more, PCA995xA and PCA9685 have the same reset sequence. This means that resetting one will reset another.
Due to these circumstances it is dependent to the system design how emitting `reset()` affects the system behavior.
 - `scanDevices()` first uses `reset()` before starting the scan.
 - After resetting you need to redo `begin()` as `reset()` does not call `init()` after reset.

## To Add More Devices

You can add other device by deriving a concrete class from `PCA9xxxPWM` or its subclass. When doing this, you also need to

- Provide your own `isMyDevice()` and call it from `PCA9xxxPWM::isMyDevice()` or subclass's `isMyDevice()`.
- Modify `PCA9xxxPWMFactory::scanDevice()` to spawn this instance.

# License

The MIT License (MIT) Copyright (c) K. Chinzei (kchinzei@gmail.com) Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
