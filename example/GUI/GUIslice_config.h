#ifndef _GUISLICE_CONFIG_H_
#define _GUISLICE_CONFIG_H_

// =======================================================================
// GUIslice library (user configuration) selection by device
// - Calvin Hass
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
// =======================================================================
//
// The MIT License
//
// Copyright 2016-2020 Calvin Hass
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =======================================================================


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Support the loading of config settings through compiler flags
// - This is used to support environments such as PlatformIO
#ifdef USER_CONFIG_LOADED
  // If a config file has been specified, the following will load it
  // Otherwise, all settings must be provided via build flags
  #ifdef USER_CONFIG_INC_FILE
    #include USER_CONFIG_INC_FNAME
  #endif
#else

  #include "../configs/esp-tftespi-default-notouch.h"

#endif // USER_CONFIG_LOADED

// =========================================================================================
// DETECT DEVICE PLATFORM
// =========================================================================================

// Detect device platform
#if defined(__linux__)
  #define GSLC_CFG_LINUX
#elif defined(__AVR__) && !defined(TEENSYDUINO)
  // Note: Teensy 2 also defines __AVR__, so differentiate with TEENSYDUINO
  #define GSLC_CFG_ARD
#elif defined(ARDUINO_SAMD_ZERO)
  #define GSLC_CFG_ARD
#elif defined(ESP8266) || defined(ESP32)
  #define GSLC_CFG_ARD
#elif defined(NRF52)
  #define GSLC_CFG_ARD
#elif defined(ARDUINO_STM32_FEATHER) || defined(__STM32F1__)
  #define GSLC_CFG_ARD
#elif defined(ARDUINO_ARCH_STM32)  // ST Core from STMicroelectronics
  #define GSLC_CFG_ARD
#elif defined(ARDUINO_ARCH_SAM)   // Arduino Due
  #define GSLC_CFG_ARD
#elif defined(ARDUINO_ARCH_SAMD)   // M0_PRO
  #define GSLC_CFG_ARD
#elif defined(__AVR__) && defined(TEENSYDUINO) // Teensy 2
  #define GSLC_CFG_ARD
  #define GSLC_DEV_TEENSY
  #define GSLC_DEV_TEENSY_2
#elif defined(__MKL26Z64__) // Teensy LC
  #define GSLC_CFG_ARD
  #define GSLC_DEV_TEENSY
  #define GSLC_DEV_TEENSY_LC
#elif defined(__MK20DX256__) // Teensy 3.2
  #define GSLC_CFG_ARD
  #define GSLC_DEV_TEENSY
  #define GSLC_DEV_TEENSY_3_2
#elif defined(__MK64FX512__) // Teensy 3.5
  #define GSLC_CFG_ARD
  #define GSLC_DEV_TEENSY
  #define GSLC_DEV_TEENSY_3_5
#elif defined(__MK66FX1M0__) // Teensy 3.6
  #define GSLC_CFG_ARD
  #define GSLC_DEV_TEENSY
  #define GSLC_DEV_TEENSY_3_6
#elif defined(__IMXRT1062__) // Teensy 4.0
  //#elif defined(ARDUINO_TEENSY40)
  //#elif defined(ARDUINO_TEENSY41)
  #define GSLC_CFG_ARD
  #define GSLC_DEV_TEENSY
  #define GSLC_DEV_TEENSY_4_0
#else
  #warning Unknown device platform
#endif


#ifndef USER_CONFIG_LOADED
  #if !defined(_GUISLICE_CONFIG_ARD_H_) && !defined(_GUISLICE_CONFIG_LINUX_H_)
    #error No config selected in GUIslice_config.h. Please uncomment/select a config.
    #error For details: https://github.com/ImpulseAdventure/GUIslice/wiki/Select-a-Config
  #endif
#endif // USER_CONFIG_LOADED

// -----------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GUISLICE_CONFIG_H_