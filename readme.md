# rx5808_etheli_diversity' Firmware for RX5808 Diversity Modules
Emphasis is on scanner functionality.

## Hardware

affordable arduino based 'openpilot' diversity board with 2 RX5808 modules (approx 15$ in 2021). Runs on either 5V or 2S-3S lipo (but onboard LDO gets really hot) use a large 2.43" ssd1309 128x64 OLED as dropin replacement for 0.96 ssd1306 128x64 OLED

## Features

* mostly identical to base etheli version
* Scanner is enhanced
 * to work on frequency instead of bands/channels.
 * uses most screen estate for graphing
 * maximum range is from [5505 ; 5995] MHz
 * scans and displays 120 steps
 * up-button increases lower range boundary,
 * down-button decreases upper range boundary
* Setup is likely broken atm

1[v13d](v13d.jpg)

## previous info

This "etheli" version of the open-source firmware for RX5808 Diversity modules (that plug into Fatshark Dominator goggles) is an enhancement to the RX5808 Diversity RealAcc Compatible firmware.  The main improvement is the addition of a BY-MHZ MODE that allows an arbitrary frequency in MHz to be directly dialed in and tuned.  Other enhancements and improvements are implemented; see the list below.

## Acknowledgments:  This code is an enhancement of the open-source code posted by [ethomas997], Shea Ivey and GC9N, and it would not exist without their hard work.
