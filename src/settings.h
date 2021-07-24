/*
 * Setings file 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <avr/pgmspace.h>
#ifndef settings_h
#define settings_h

#define PROG_REVISION_STR "RX5808 v1.3d"

// #define Debug

// Choose the display you will be using
// you will also have to uncomment the includes in the main project.
//#define TVOUT_SCREENS
#define OLED_128x64_ADAFRUIT_SCREENS

// use the library from https://github.com/badzz/Adafruit_SH1106 before enabling
//#define SH1106

// u8glib has performance issues.
//#define OLED_128x64_U8G_SCREENS
 

// this will be displayed on the screensaver.
// Up to 10 letters
#define CALL_SIGN "CS"


// Feature Toggles
#define USE_DIVERSITY

// uncomment to enable OSD support by GC9N
//#define USE_GC9N_OSD

//#define USE_FLIP_SCREEN
// #define USE_BOOT_LOGO

// Choose if you wish to use 8 additional Channels 
// 5362 MHz 5399 MHz 5436 MHz 5473 MHz 5510 MHz 5547 MHz 5584 MHz 5621 MHz
// Local laws may prohibit the use of these frequencies use at your own risk!
//#define USE_LBAND

// Receiver Module version
// used for tuning time
#define rx5808
//#define rx5880
 
#define spiDataPin 10
#define slaveSelectPin 11
#define spiClockPin 12

// Receiver PINS
#define receiverA_led A0
//Feature fast switching breaks the changeability of receiverA_led and receiverB_led, only to be used when receiverA_led = A0 and receiverB_led = A1
#define USE_FAST_SWITCHING
#define rssiPinA A6

#define useReceiverA 1

#ifdef USE_DIVERSITY
    // Diversity
    #define receiverB_led A1
    #define rssiPinB A7
    #define useReceiverAuto 0
    #define useReceiverB 2   
    // rssi strength should be 2% greater than other receiver before switch.
    // this prevents flicker when rssi values are close and delays diversity checks counter.
    
    #define DIVERSITY_CUTOVER 4 // changing this to 4% to try making it smoother

    // Original comments number of checks a receiver needs to win over the other to switch receivers.
    // this prevents rapid switching.

    // 1 to 10 is a good range. 1 being fast switching, 10 being slow 100ms to switch.
    #define DIVERSITY_MAX_CHECKS 7 //changing this to 7 try making it smoother eliminate sync problems.
#endif

// this two are minimum required
#define buttonUp 4 //2
#define buttonMode 3
// optional comfort buttons
#define buttonDown 2 //4
#define buttonSave 5
// Buzzer
#define buzzer 6

// key debounce delay in ms
// NOTE: good values are in the range of 100-200ms
// shorter values will make it more reactive, but may lead to double trigger
#define KEY_DEBOUNCE 200

#define led 13
// number of analog rssi reads to average for the current check.
#define RSSI_READS 10
// RSSI default raw range
#define RSSI_MIN_VAL 75 //90
#define RSSI_MAX_VAL 215 //220
// 75% threshold, when channel is printed in spectrum
#define RSSI_SEEK_FOUND 50
// RSSI value for channel found during auto-seek
#define RSSI_SEEK_TRESHOLD 1 //60
// scan loops for setup run
#define RSSI_SETUP_RUN 3

#define STATE_SEEK_FOUND 0
#define STATE_SEEK 1
#define STATE_SCAN 2
#define STATE_MANUAL 3
#define STATE_FREQ_BYMHZ 4
#define STATE_SETUP_MENU 5
#define STATE_SAVE 6
#ifdef USE_DIVERSITY
#define STATE_DIVERSITY 7
#endif
#define STATE_RSSI_SETUP 8
#define STATE_SCREEN_SAVER 9
#define STATE_FAVORITE 10 //gc9n
#define STATE_SCREEN_SAVER_LITE 11
#define STATE_MAX_VALUE STATE_SCREEN_SAVER_LITE

#define START_STATE STATE_SCAN

// Seconds to wait before force entering screensaver
#define SCREENSAVER_TIMEOUT 6

#define CHANNEL_BAND_SIZE 8
#define CHANNEL_MIN_INDEX 0
#ifdef USE_LBAND
    #define CHANNEL_MAX_INDEX 47
#else
    #define CHANNEL_MAX_INDEX 39
#endif

#ifdef rx5808
    // rx5808 module need >20ms to tune.
    // 25 ms will do a 40 channel scan in 1 second.
    // 35 ms will do a 40 channel scan in 1.4 seconds.
    #define MIN_TUNE_TIME 35
#endif
#ifdef rx5880
    // rx5880 module needs >30ms to tune.
    // 35 ms will do a 40 channel scan in 1.4 seconds.
    #define MIN_TUNE_TIME 35
#endif

#ifdef USE_LBAND
    #define CHANNEL_MAX 47
#else
    #define CHANNEL_MAX 39
#endif
#define CHANNEL_MIN 0

#define MIN_CHANNEL_MHZ 5000      //min MHz value for Set by MHz mode
#define MAX_CHANNEL_MHZ 5999      //max MHz value for Set by MHz mode

#define BARS_COUNT (119)
#define MHZ_RANGE_MIN 5505 // 5645
#define MHZ_RANGE_MAX 5995 // 5945

#define TICKER 50 // in bargraph, show axis marking every 50MHz

#ifdef USE_DIVERSITY
// used to figure out if diversity module has been plugged in.
// When RSSI is plugged in the min value is around 90
// When RSSI is not plugged in the min value is 0
#define isDiversity() (analogRead(rssiPinB) >= 5)
#endif

#endif // file_defined
