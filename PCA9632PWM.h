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
  Part of this source code uses
  https://os.mbed.com/users/nxp_ip/code/PCA962x/
  Original header is below. Note it's Apache 2 License.
*/

/** PCA9632 PWM control LED driver
 *
 *  An operation sample of PCA9632 4-channel Fm+ I2C-bus 100mA/40 V LED driver.
 *  mbed accesses the PCA9632 registers through I2C.
 *
 *  @class   PCA9632
 *  @author  Akifumi (Tedd) OKANO, NXP Semiconductors
 *  @version 0.6
 *  @date    04-Mar-2015
 *
 *  Released under the Apache 2 license
 *
 *  About PCA9632:
 *    http://www.nxp.com/products/lighting_driver_and_controller_ics/i2c_led_display_control/series/PCA9632.html
 */

#ifndef _PCA9632_PWM_H_
#define _PCA9632_PWM_H_

#include "PCA962xPWM.h"

class PCA9632PWM : public PCA962xPWM {
public:
  enum command_reg {
    MODE1,      /**< MODE1 register      */
    MODE2,      /**< MODE2 register      */
    PWM0,       /**< PWM0 register       */
    PWM1,       /**< PWM1 register       */
    PWM2,       /**< PWM2 register       */
    PWM3,       /**< PWM3 register       */
    GRPPWM,     /**< GRPPWM register     */
    GRPFREQ,    /**< GRPFREQ register    */
    LEDOUT,     /**< LEDOUT register    */
    SUBADR1,    /**< SUBADR1 register    */
    SUBADR2,    /**< SUBADR2 register    */
    SUBADR3,    /**< SUBADR3 register    */
    ALLCALLADR, /**< ALLCALLADR register */

    REGISTER_START = MODE1,
    LEDOUT_REGISTER_START = LEDOUT,
    PWM_REGISTER_START = PWM0,
  };

  PCA9632PWM(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);
  virtual ~PCA9632PWM();

  boolean hasBegun();
  void customHasBegun();

  String type_name() const;

  uint8_t number_of_ports() const;

  static boolean isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);

  static String class_type() { return "PCA9632"; };

private:
  void init();
  uint8_t pwm_register_access(uint8_t port);
  static boolean _isMyDevice(uint8_t i2cAddr, TwoWire *i2cPort = &Wire);

  const uint8_t n_of_ports;
};

#endif
