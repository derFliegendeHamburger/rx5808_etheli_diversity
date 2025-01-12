/*
   OLED Screens by Shea Ivey
 
  The MIT License (MIT)
 
  Copyright (c) 2015 Shea Ivey
  **MODS Geroge chatzisavvidis
  
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
 
#include "settings.h"
#include <avr/pgmspace.h>
#ifdef OLED_128x64_ADAFRUIT_SCREENS
#include "screens.h" // function headers
 
#include "Adafruit_SSD1306.h"
 
#include "Adafruit_GFX.h"
#include <Wire.h>
#include <SPI.h>
 
// New version of PSTR that uses a temp buffer and returns char *
// by Shea Ivey
#define PSTR2(x) PSTRtoBuffer_P(PSTR(x))
char PSTR2_BUFFER[30]; // adjust size depending on need.
char *PSTRtoBuffer_P(PGM_P str)
{
  uint8_t c = '\0', i = 0;
  for (; (c = pgm_read_byte(str)) && i < sizeof(PSTR2_BUFFER); str++, i++) PSTR2_BUFFER[i] = c;
  PSTR2_BUFFER[i] = c;
  return PSTR2_BUFFER;
}
 
#define INVERT INVERSE
#define OLED_RESET 4
#ifdef SH1106
Adafruit_SH1106 display(OLED_RESET);
#if !defined SH1106_128_64
#error("Screen size incorrect, please fix Adafruit_SH1106.h!");
#endif
#else
Adafruit_SSD1306 display(OLED_RESET);
#if !defined SSD1306_128_64
#error("Screen size incorrect, please fix Adafruit_SSD1306.h!");
#endif
#endif


extern uint8_t system_state;

 
screens::screens()
{
  last_channel = -1;
  last_rssi = 0;
  best_rssi = 0;
  bestChannelName = 0;
  bestChannelFrequency = 0;
}
 
char screens::begin(const char *call_sign)
{
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D or 0x3C (for the 128x64)
 
 
#ifdef USE_FLIP_SCREEN
  flip();
#endif
 
#ifdef USE_BOOT_LOGO
  display.display(); // show splash screen
  delay(2000);
#endif
  // init done
  reset();
 
  return 0; // no errors
}
 
void screens::reset()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}
 
void screens::flip()
{
  display.setRotation(2);
}
 
void screens::drawTitleBox(const char *title, bool centerFlag)
{
  display.drawRect(0, 0, display.width(), display.height(), WHITE);
  display.fillRect(0, 0, display.width(), 11, WHITE);
 
  display.setTextSize(1);
  display.setTextColor(BLACK);

  if (centerFlag)
    display.setCursor(((display.width() - (strlen(title) * 6)) / 2), 2);
  else
    display.setCursor(2, 2);

  display.print(title);
  
  display.setTextColor(WHITE);
}
 
void screens::mainMenuSecondPage(uint8_t menu_id, bool settings_OSD)
{
  reset(); // start from fresh screen.
  drawTitleBox(PSTR2(PROG_REVISION_STR "  MENU 2"), false);
  display.fillRect(0, 10 * menu_id + 12, display.width(), 10, WHITE);
 
  display.setTextColor(menu_id == 0 ? BLACK : WHITE);
  display.setCursor(5, 10 * 0 + 13);
  display.print(PSTR2("SETUP MENU"));
  display.print(PSTR2("         "));
  display.write(24);         // display up-arrow symbol
 
//  display.setTextColor(menu_id == 0 ? BLACK : WHITE);
//  display.setCursor(5, 10 * 1 + 13);
//  display.print(PSTR2(" "));

#ifdef USE_DIVERSITY
  if (isDiversity())
  {
    display.setTextColor(menu_id == 1 ? BLACK : WHITE);
    display.setCursor(5, 10 * 1 + 13);
    display.print(PSTR2("DIVERSITY"));
  }
#endif

  display.setTextColor(menu_id == 2 ? BLACK : WHITE);
  display.setCursor(5, 10 * 2 + 13);
  display.print(PSTR2("FIND MODEL"));

#ifdef USE_GC9N_OSD
  display.setTextColor(menu_id == 3 ? BLACK : WHITE);
  display.setCursor(5, 10 * 3 + 13);
  display.print(PSTR2("OSD: "));
  if (settings_OSD) 
    display.print(PSTR2("ON "));
  else 
    display.print(PSTR2("OFF"));
#endif

  display.display();
}
 
void screens::mainMenu(uint8_t menu_id)
{
  reset(); // start from fresh screen.
  drawTitleBox(PSTR2(PROG_REVISION_STR "  MENU 1"), false);
  display.fillRect(0, 10 * menu_id + 12, display.width(), 10, WHITE);
  display.setTextColor(menu_id == 0 ? BLACK : WHITE);
  display.setCursor(5, 10 * 0 + 13);
  display.print(PSTR2("AUTO MODE"));
  display.setTextColor(menu_id == 1 ? BLACK : WHITE);
  display.setCursor(5, 10 * 1 + 13);
  display.print(PSTR2("BAND SCANNER"));
  display.setTextColor(menu_id == 2 ? BLACK : WHITE);
  display.setCursor(5, 10 * 2 + 13);
  display.print(PSTR2("MANUAL MODE"));

  display.setTextColor(menu_id == 3 ? BLACK : WHITE);
  display.setCursor(5, 10 * 3 + 13);
  display.print(PSTR2("BY-MHZ MODE"));

  display.setTextColor(menu_id == 4 ? BLACK : WHITE);
  display.setCursor(5, 10 * 4 + 13);
  display.print(PSTR2("FAVORITES"));
  display.print(PSTR2("         "));
  display.write(25);         // display down-arrow symbol
  display.display();
}
 
void screens::seekMode(uint8_t state)
{
  last_channel = -1;
  reset(); // start from fresh screen.
  if (state == STATE_FAVORITE)
  {
    drawTitleBox(PSTR2("FAVORITE MODE"));
  }
  if (state == STATE_MANUAL)
  {
    drawTitleBox(PSTR2("MANUAL MODE"));
  }
  else if (state == STATE_SEEK)
  {
    drawTitleBox(PSTR2("AUTO SEEK MODE"));
  }
  display.setTextColor(WHITE);
  display.drawFastHLine(0, 20, display.width(), WHITE);
  display.drawFastHLine(0, 32, display.width(), WHITE);
  display.setCursor(5, 12);
  display.drawLine(97, 11, 97, 20, WHITE);
  display.print(PSTR2("BAND:"));
  for (uint16_t i = 0; i < 8; i++)
  {
    display.setCursor(15 * i + 8, 23);
    display.print((char) (i + '1'));
  }
  display.drawLine(0, 36, display.width(), 36, WHITE);
  display.drawLine(0, display.height() - 11, display.width(), display.height() - 11, WHITE);
  display.setCursor(2, display.height() - 9);
#ifdef USE_LBAND
  display.print(PSTR2("5362"));
#else
  display.print(PSTR2("5645"));
#endif
  display.setCursor(55, display.height() - 9);
  display.print(PSTR2("5800"));
  display.setCursor(display.width() - 25, display.height() - 9);
  display.print(PSTR2("5945"));
  display.display();
}
 
void screens::FavDelete( uint16_t channelFrequency, uint8_t channel)
{
  reset(); // start from fresh screen.
  drawTitleBox(PSTR2("DELETE FAVORITE CHAN"));
  display.setTextColor(WHITE);
  display.setCursor(5, 8 * 1 + 4);
  display.print(PSTR2("CHANNEL FREQ:"));
  display.setCursor(85, 8 * 1 + 4);
  display.print((channelFrequency));
  display.setCursor(5, 8 * 2 + 4);
  display.print(PSTR2("FAV CHANNEL:"));
  display.setCursor(85, 8 * 2 + 4);
  display.print(channel);
 
  display.setCursor(((display.width() - 11 * 6) / 2), 8 * 6 + 4);
  display.print(PSTR2("-- DELETED --"));
  display.display();
}
 
void screens::FavSel(uint8_t favchan) 
{//gc9n
  reset(); // start from fresh screen.
  drawTitleBox(PSTR2("SELECTED FAVORITE CH"));
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.print(favchan);
  
  display.display();
  reset();
}
 
//void screens::FavReorg(uint8_t favchan) 
// {//gc9n
//  reset(); // start from fresh screen.
//  drawTitleBox(PSTR2("REORGANIZING")); 
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(5, 8 * 2 + 4);
//  display.print(PSTR2("REORGANIZING FAV/ES "));
//  display.setCursor(((display.width() - 11 * 6) / 2), 8 * 4 + 4);
//  display.print(PSTR2("TOTAL FAV/ES="));
//  display.print(favchan);
// 
//  display.display();
//  reset();
//  reset();
//  delay(1500);
//}
 
void screens::NoFav()
{
  reset(); // start from fresh screen.
  drawTitleBox(PSTR2("FAVORITES"));
 
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.print(PSTR2("EMPTY"));
  display.display();
}
 
char scan_position = 3;
 
void screens::updateSeekMode(uint8_t state, uint8_t channelIndex, uint8_t channel, uint8_t rssi, uint16_t channelFrequency, uint8_t rssi_seek_threshold, bool locked)
{
  // display refresh handler
  if (channel != last_channel) // only updated on changes
  {
    if (channel > last_channel) {
      scan_position = 3;
    }
    else {
      scan_position = -1;
    }
    display.setTextColor(WHITE, BLACK);
    display.setCursor(36, 12);
    // show current used channel of bank
#ifdef USE_LBAND
    if (channelIndex > 39)
    {
      display.print(PSTR2("L/Lowrace"));
    }
    else if (channelIndex > 31)
#else
    if (channelIndex > 31)
#endif
    {
      display.print(PSTR2("R/Racebnd"));
    }
    else if (channelIndex > 23)
    {
      display.print(PSTR2("F/Airwave"));
    }
    else if (channelIndex > 15)
    {
      display.print(PSTR2("E        "));
    }
    else if (channelIndex > 7)
    {
      display.print(PSTR2("B        "));
    }
    else
    {
      display.print(PSTR2("A        "));
    }
 
    uint8_t active_channel = channelIndex % CHANNEL_BAND_SIZE; // get channel inside band
    for (int i = 0; i < 8; i++) {
      display.fillRect(15 * i + 4, 21, 14, 11, i == active_channel ? WHITE : BLACK);
      display.setTextColor(i == active_channel ? BLACK : WHITE);
      display.setCursor(15 * i + 8, 23);
      display.print((char) (i + '1'));
    }
 
    // show frequence
    display.setCursor(101, 12);
    display.setTextColor(WHITE, BLACK);
    display.print(channelFrequency);
  }
  // show signal strength
  uint8_t rssi_scaled = map(rssi, 1, 100, 1, display.width() - 3);
 
  display.fillRect(1 + rssi_scaled, 33, display.width() - 3 - rssi_scaled, 3, BLACK);
  display.fillRect(1, 33, rssi_scaled, 3, WHITE);
 
  rssi_scaled = map(rssi, 1, 100, 1, 14);
#ifdef USE_LBAND
  display.fillRect((channel * 3) + 4, display.height() - 12 - 14, 5 / 2, 14 - rssi_scaled, BLACK);
  display.fillRect((channel * 3) + 4, (display.height() - 12 - rssi_scaled), 5 / 2, rssi_scaled, WHITE);
#else
  display.fillRect((channel * 3) + 4, display.height() - 12 - 14, 3, 14 - rssi_scaled, BLACK);
  display.fillRect((channel * 3) + 4, (display.height() - 12 - rssi_scaled), 3, rssi_scaled, WHITE);
#endif
 
  // handling for seek mode after screen and RSSI has been fully processed
  if (state == STATE_SEEK) //
  { // SEEK MODE
 
    // Show Scan Position
#ifdef USE_LBAND
    display.fillRect((channel * 5 / 2) + 4 + scan_position, display.height() - 12 - 14, 1, 14, BLACK);
#else
    display.fillRect((channel * 3) + 4 + scan_position, display.height() - 12 - 14, 1, 14, BLACK);
#endif
 
    rssi_scaled = map(rssi_seek_threshold, 1, 100, 1, 14);
    
    display.fillRect(1, display.height() - 12 - 14, 2, 14, BLACK);
    display.drawLine(1, display.height() - 12 - rssi_scaled, 2, display.height() - 12 - rssi_scaled, WHITE);
    display.fillRect(display.width() - 3, display.height() - 12 - 14, 2, 14, BLACK);
    display.drawLine(display.width() - 3, display.height() - 12 - rssi_scaled, display.width(), display.height() - 12 - rssi_scaled, WHITE);
 
    if (locked) // search if not found
    {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(((display.width() - 14 * 6) / 2), 2);
      display.print(PSTR2("AUTO MODE LOCK"));
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(((display.width() - 14 * 6) / 2), 2);
      display.print(PSTR2("AUTO SEEK MODE"));
    }
  }
 
  last_channel = channel;
  display.display();
}
 
extern  uint16_t mhz_range_min;
extern  uint16_t mhz_range_max;
// display top left is (0,0)
#define BAR_BASE_Y (52) //  this defines visual bottom base line for the bargraph
#define BAR_HEIGHT 50 // relative height of the bargraph
#define BAR_TOP_Y (BAR_BASE_Y - BAR_HEIGHT +1) // this defines visual top line for the bargraph
#define BAR_OFFSET_X 4 // absolute first used x coordinate for bargraph
#define LASTLINE_Y (display.height() - 8)

#define FREQLINE_Y (BAR_TOP_Y +20) // top corner for moving frequency display (4 chars)
#define FREQLINE_MIN_X (18)	// right most position for moving frequency text
#define FREQLINE_MAX_X (display.width() - 51)	// right most position for moving frequency text
void screens::bandScanMode(uint8_t state)
{
  reset(); // start from fresh screen.
  best_rssi = 0;
  bestChannelName = 0;
  bestChannelFrequency = 0;
  if (state == STATE_SCAN)
  {
    //drawTitleBox(PSTR2("BAND SCANNER"));
    //display.setCursor(0, 0);
    //display.print(PSTR2("BEST:"));
  }
  else
  {
    drawTitleBox(PSTR2("RSSI SETUP"));
    display.setCursor(5, 12);
    display.print(PSTR2("Min:     Max:"));
  }
  //display.drawLine(0, 20, display.width(), 20, WHITE);
 
  //display.drawLine(0, display.height() - 11, display.width(), display.height() - 11, WHITE);
  display.setCursor(2, LASTLINE_Y);
  display.print(mhz_range_min);
  //display.setCursor(55, LASTLINE_Y);
  //display.print(PSTR2("5800"));
  display.setCursor(display.width() - 25, LASTLINE_Y);
  display.print(mhz_range_max);
  display.display();
}
 
extern int rawRssiA, rawRssiB;
void print3Digits(int i) { if (i<100) display.print(' '); if (i<10) display.print(' '); display.print(i); }

void screens::updateBandScanMode(bool in_setup, uint8_t channel, uint8_t rssi, uint16_t channelName, uint16_t channelFrequency, uint16_t rssi_setup_min_a, uint16_t rssi_setup_max_a)
{
  static uint8_t freqline_y = FREQLINE_Y;
  static uint8_t ftextx = 64;
  static uint8_t fpercx = 64;
  static uint8_t bcfx = 64;
  static boolean below = true;
  static uint8_t f=64; // x-position of the maxed frequency
  static uint8_t h=32; // y-position of the 'half amplitude' mark
  uint8_t rssi_scaled = rssi / 2; //map(rssi, 1, 100, 1, BAR_HEIGHT);
  uint16_t hight = BAR_BASE_Y - rssi_scaled; // y-position for the current rssi value
  if (channel != last_channel) // only updated on changes
  {
#ifdef USE_LBAND
    display.fillRect((channel * 5 / 2) + 4, display.height() - 12 - BAR_MAX_Y, 5 / 2, BAR_MAX_Y - rssi_scaled, BLACK);
    display.fillRect((channel * 5 / 2) + 4, hight, 5 / 2, rssi_scaled, WHITE);
    // Show Scan Position
    display.fillRect((channel * 5 / 2) + 4 + 3, display.height() - 12 - BAR_MAX_Y, 1, BAR_MAX_Y, BLACK);
#elif 0
    display.fillRect((channel * 3) + 4, display.height() - 12 - BAR_MAX_Y, 3, BAR_MAX_Y - rssi_scaled, BLACK);
    display.fillRect((channel * 3) + 4, hight, 3, rssi_scaled, WHITE);
    // Show Scan Position
    /*if (channel < CHANNEL_MAX)*/ display.fillRect((channel * 3) + 4 + 3, display.height() - 12 - BAR_MAX_Y, 1, BAR_MAX_Y, BLACK);
#endif
    uint8_t x = channel + BAR_OFFSET_X;
    display.drawLine(x, 0,  x, hight-1, BLACK); // from top of display
    display.drawLine(x, hight,  x, BAR_BASE_Y, WHITE);
    // Show Scan Position
    display.drawLine(x+1, 0,  x+1, BAR_BASE_Y, INVERSE); // from top of display

    if ( (channelFrequency % TICKER < TICKER/2) ) {
    	if (below) {
    		display.drawLine(x, BAR_BASE_Y, x, BAR_BASE_Y+3, WHITE);
    		below = false;
    	}
    }
    else
    {
    	below = true;
    }
  }

  if (!in_setup)
  {
    // if scan started new sweep then show best channel from last sweep
    //  and setup to look for new best-RSSI channel
    if (channel < last_channel)
    {
      if (bestChannelName > 0)
      {  //new best-RSSI channel was found
        display.setTextColor(WHITE, BLACK);
        display.setCursor(fpercx, 0); // erase previous %display
        display.print(PSTR2("   "));
        display.setCursor(bcfx, LASTLINE_Y); // erase previous 'bestFreq' display
        display.print(PSTR2("    "));
        display.setCursor(FREQLINE_MAX_X, freqline_y); // erase rightmost moving freq
        display.print(PSTR2("     "));
        // new pos calculations:
		f = BAR_OFFSET_X + best_channel;
		ftextx = min(f, FREQLINE_MAX_X)+3; // x pos of maxed text
		fpercx = (f<64 ? min(f+10, FREQLINE_MAX_X)+3 : max(f-40, FREQLINE_MIN_X) ); // x pos of maxed percentage text
		bcfx = max(FREQLINE_MIN_X+11, min(FREQLINE_MAX_X-1,best_channel + BAR_OFFSET_X -11 /*+3*/));
        h = BAR_BASE_Y - best_rssi/4;
        freqline_y = (h > BAR_BASE_Y -15 ? FREQLINE_Y :h - 9);
        display.setCursor(fpercx, 0);
        //display.print((char)(bestChannelName >> 8));    //band char
        //display.print((char)(bestChannelName & 0xFF));  //channel char
        display.print(best_rssi); display.print('%');
		display.setCursor(bcfx, LASTLINE_Y);
        display.print(bestChannelFrequency);
        display.setCursor(0, 3);
        print3Digits(copy_bestRawRssiA);
        display.setCursor(107, 3);
        print3Digits(copy_bestRawRssiB);
        bestChannelName = 0;
        bestChannelFrequency = 0;
      }
      copy_best_rssi = best_rssi;
      best_rssi = 0;
      copy_bestRawRssiA = bestRawRssiA;
      bestRawRssiA = 0;
      copy_bestRawRssiB = bestRawRssiB;
      bestRawRssiB = 0;
      display.drawLine(BARS_COUNT+ BAR_OFFSET_X+1, BAR_TOP_Y,  BARS_COUNT+ BAR_OFFSET_X+1, BAR_BASE_Y, BLACK); // erase rightmost inverse scan position bar
    }
    if (channel <= 15 ) {
    	// repair display of bestRawRssiA
        display.setCursor(0, 3);
        print3Digits(copy_bestRawRssiA);
    }
    if (channel >= 107 - BAR_OFFSET_X ) {
     	// repair display of bestRawRssiB
         display.setCursor(107, 3);
         print3Digits(copy_bestRawRssiB);
    }
    if (channel >= fpercx && channel < fpercx+30) {
     	// repair display of percent
        display.setCursor(fpercx, 0);
        //display.print((char)(bestChannelName >> 8));    //band char
        //display.print((char)(bestChannelName & 0xFF));  //channel char
        //display.print(' ');
        display.print(copy_best_rssi); display.print('%');
     }
	display.setCursor(0, 12);
	print3Digits(rawRssiA);
	display.setCursor(max(FREQLINE_MIN_X, min(FREQLINE_MAX_X,channel + BAR_OFFSET_X +3)), freqline_y);
	display.print(' '); display.print(channelFrequency);
	display.setCursor(107, 12);
	print3Digits(rawRssiB);
	display.drawLine(f,BAR_TOP_Y,f, 10 + BAR_BASE_Y, INVERSE); // location of max freq
	if (h < BAR_BASE_Y -15) display.drawLine(4,h,124,h, INVERSE); // level of half best_rssi
    if (rssi > RSSI_SEEK_TRESHOLD)
    {
		if (rssi > best_rssi) {
			best_rssi = rssi;
			best_channel = channel;
			bestChannelName = channelName;
			bestChannelFrequency = channelFrequency;
			bestRawRssiA = rawRssiA;
			bestRawRssiB = rawRssiB;
		} else {
		}
    }
  } // (!in_setup
  else
  { // in_setup
    display.setCursor(30, 12);
    display.setTextColor(WHITE, BLACK);
    display.print( PSTR2("   ") );
    display.setCursor(30, 12);
    display.print( rssi_setup_min_a , DEC);
 
    display.setCursor(85, 12);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(85, 12);
    display.print( PSTR2("   ") );
    display.print( rssi_setup_max_a , DEC);
  }
  display.display();
  last_channel = channel;
}
 
void screens::screenSaver(uint16_t channelName, uint16_t channelFrequency, const char *call_sign)
{
  screenSaver(-1, channelName, channelFrequency, call_sign);
}

void screens::screenSaver(uint8_t diversity_mode, uint16_t channelName, uint16_t channelFrequency, const char *call_sign)
{
  reset();
//  display.setTextSize(6);
//  display.setTextColor(WHITE);
//  display.setCursor(0, 0);
//  display.print(channelName, HEX);
//  display.setTextSize(1);
//  display.setCursor(70, 0);
//  display.print(call_sign);
//  display.setTextSize(2);
//  display.setCursor(70, 28);
//  display.setTextColor(WHITE);
//  display.print(channelFrequency);
//  display.setTextSize(1);
 
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(2, 0);
  display.print(channelFrequency);
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.print(call_sign);
  display.setTextSize(2);
  display.setCursor(98, 2);
  display.setTextColor(WHITE);

  if (channelName > 0)
  {
    display.print((char)(channelName >> 8));       //band char
    display.print((char)(channelName & 0xFF));     //channel char
  }

  display.setTextSize(1);
  display.drawLine(0,27,50,27,0);
 
 
#ifdef USE_DIVERSITY
  if (isDiversity()) 
  {
    
  // display.print("B");
   display.setCursor(98, 20);
   display.setTextColor(WHITE, BLACK);
    switch (diversity_mode) 
    {
      case useReceiverAuto:
        display.print(PSTR2("AUTO"));
        break;
      case useReceiverA:
        display.print(PSTR2("ANT A"));
        break;
      case useReceiverB:
        display.print(PSTR2("ANT B"));
        break;
    }
  }
#endif
 
 
  display.setTextColor(BLACK, WHITE);
  display.fillRect(0, display.height() - 27, 7,13 , WHITE);
  display.setCursor(1, display.height() - 24);
  display.print(PSTR2("A"));
  display.write(10);
  display.setTextColor(BLACK, WHITE);
  display.fillRect(0, display.height() - 13, 7, 13, WHITE);
  display.setCursor(1, display.height() - 11);
  display.print(PSTR2("B"));  
  display.display();
}
 
void screens::updateScreenSaver(uint8_t rssi)
{
  updateScreenSaver(-1, rssi, -1, -1 );
}

void screens::updateScreenSaver(char active_receiver, uint8_t rssi, uint8_t rssiA, uint8_t rssiB)
{
  
  if (system_state == STATE_SCREEN_SAVER_LITE)
  {
    reset(); // start from fresh screen.
    uint8_t rssi_scaled = map(rssiA, 1, 100, 3, 119);
    display.fillRect(0, 30, rssi_scaled, 30, WHITE);
    display.drawRect(0, 30, 119, 30, WHITE);
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(20, 0);
    display.print(rssiA);
    display.print(PSTR2("% A"));
    display.setCursor(0, 0);
    uint8_t character;

    if (rssiA<50)
      character = 25;
    else
      character = 24;
    display.write(character);
  }
  else
  {
    #ifdef USE_DIVERSITY
      if (isDiversity())
      {
        // read rssi A
          #define RSSI_BAR_SIZE 119
        uint8_t rssi_scaled = map(rssiA, 1, 100, 3, RSSI_BAR_SIZE);
        display.fillRect(7 + rssi_scaled, display.height() - 27, (RSSI_BAR_SIZE - rssi_scaled), 13, BLACK);
        if (active_receiver == useReceiverA)
        {
          display.fillRect(7, display.height() - 27, rssi_scaled, 13, WHITE);
        }
        else
        {
          display.fillRect(7, display.height() - 27, (RSSI_BAR_SIZE), 13, BLACK);
          display.drawRect(7, display.height() - 27, rssi_scaled, 13, WHITE);
        }
     
        // read rssi B
        rssi_scaled = map(rssiB, 1, 100, 3, RSSI_BAR_SIZE);
        display.fillRect(7 + rssi_scaled, display.height() - 13, (RSSI_BAR_SIZE - rssi_scaled), 13, BLACK);
        if (active_receiver == useReceiverB)
        {
          display.fillRect(7, display.height() - 13, rssi_scaled, 13, WHITE);
        }
        else
        {
          display.fillRect(7, display.height() - 13, (RSSI_BAR_SIZE), 13, BLACK);
          display.drawRect(7, display.height() - 13, rssi_scaled, 13, WHITE);
        }
      }
      else
      {
        display.setTextColor(BLACK);
        display.fillRect(0, display.height() - 27, 25, 19, WHITE);
        display.setCursor(1, display.height() - 13);
        display.print(PSTR2("RSSI"));
    #define RSSI_BAR_SIZE 101
        uint8_t rssi_scaled = map(rssi, 1, 100, 1, RSSI_BAR_SIZE);
        display.fillRect(25 + rssi_scaled, display.height() - 19, (RSSI_BAR_SIZE - rssi_scaled), 19, BLACK);
        display.fillRect(25, display.height() - 27, rssi_scaled, 19, WHITE);
      }
    #else
      display.setTextColor(BLACK);
      display.fillRect(0, display.height() - 27, 25, 19, WHITE);
      display.setCursor(1, display.height() - 13);
      display.print(PSTR2("RSSI"));
    #define RSSI_BAR_SIZE 101
      uint8_t rssi_scaled = map(rssi, 1, 100, 1, RSSI_BAR_SIZE);
      display.fillRect(25 + rssi_scaled, display.height() - 27, (RSSI_BAR_SIZE - rssi_scaled), 27, BLACK);
      display.fillRect(25, display.height() - 27, rssi_scaled, 27, WHITE);
    #endif
      if (rssi < 20)
      {
        display.setTextColor((millis() % 250 < 125) ? WHITE : BLACK, BLACK);  //250 < 125
        display.setCursor(45, display.height() - 18);
        display.print(PSTR2("LOW SIGNAL"));
      }
    #ifdef USE_DIVERSITY
      else if (isDiversity()) {
      //  display.drawLine(50, display.height() - 10, 110, display.height() - 10, BLACK);
      }
    #endif

  }
   
  display.display();
}
 
#ifdef USE_DIVERSITY
void screens::diversity(uint8_t diversity_mode)
{
 
  reset();
  drawTitleBox(PSTR2("DIVERSITY"));
 
  //selected
  display.fillRect(0, 10 * diversity_mode + 12, display.width(), 10, WHITE);
 
  display.setTextColor(diversity_mode == useReceiverAuto ? BLACK : WHITE);
  display.setCursor(5, 10 * 1 + 3);
  display.print(PSTR2("AUTO"));
 
  display.setTextColor(diversity_mode == useReceiverA ? BLACK : WHITE);
  display.setCursor(5, 10 * 2 + 3);
  display.print(PSTR2("RECEIVER A"));
  display.setTextColor(diversity_mode == useReceiverB ? BLACK : WHITE);
  display.setCursor(5, 10 * 3 + 3);
  display.print(PSTR2("RECEIVER B"));
 
  // RSSI Strength
  display.setTextColor(WHITE);
  display.drawRect(0, display.height() - 21, display.width(), 11, WHITE);
  display.setCursor(5, display.height() - 19);
     display.print(PSTR2("A:"));
  display.setCursor(5, display.height() - 9);
  display.print(PSTR2("B"));
  display.display();
}
 
void screens::updateDiversity(char active_receiver, uint8_t rssiA, uint8_t rssiB)
{
#define RSSI_BAR_SIZE 108
  uint8_t rssi_scaled = map(rssiA, 1, 100, 1, RSSI_BAR_SIZE);
 
  display.fillRect(18 + rssi_scaled, display.height() - 19, (RSSI_BAR_SIZE - rssi_scaled), 7, BLACK);
  if (active_receiver == useReceiverA)
  {
    display.fillRect(18, display.height() - 19, rssi_scaled, 7, WHITE);
  }
  else
  {
    display.fillRect(18, display.height() - 19, rssi_scaled, 7, BLACK);
    display.drawRect(18, display.height() - 19, rssi_scaled, 7, WHITE);
  }
 
  // read rssi B
  rssi_scaled = map(rssiB, 1, 100, 1, RSSI_BAR_SIZE);
  display.fillRect(18 + rssi_scaled, display.height() - 9, (RSSI_BAR_SIZE - rssi_scaled), 7, BLACK);
  if (active_receiver == useReceiverB)
  {
    display.fillRect(18, display.height() - 9, rssi_scaled, 7, WHITE);
  }
  else
  {
    display.fillRect(18, display.height() - 9, rssi_scaled, 7, BLACK);
    display.drawRect(18, display.height() - 9, rssi_scaled, 7, WHITE);
  }
  display.display();
}
#endif
 
 
void screens::setupMenu()
{
}

void screens::updateSetupMenu(uint8_t menu_id, bool settings_beeps, bool settings_orderby_channel, const char *call_sign  , char editing)
{
  reset();
  drawTitleBox(PSTR2("SETUP MENU"));
  //selected
  display.fillRect(0, 10 * menu_id + 12, display.width(), 10, WHITE);
 
  display.setTextColor(menu_id == 0 ? BLACK : WHITE);
  display.setCursor(5, 10 * 1 + 3);
  display.print(PSTR2("ORDER: "));
  if (settings_orderby_channel) {
    display.print(PSTR2("CHANNEL  "));
  }
  else {
    display.print(PSTR2("FREQUENCY"));
  }
 
  display.setTextColor(menu_id == 1 ? BLACK : WHITE);
  display.setCursor(5, 10 * 2 + 3);
  display.print(PSTR2("BEEPS: "));
  if (settings_beeps) {
    display.print(PSTR2("ON "));
  }
  else {
    display.print(PSTR2("OFF"));
  }
 
 
  display.setTextColor(menu_id == 2 ? BLACK : WHITE);
  display.setCursor(5, 10 * 3 + 3);
  display.print(PSTR2("SIGN : "));
  if (editing >= 0) {
    display.fillRect(6 * 6 + 5, 10 * 2 + 13, display.width() - (6 * 6 + 6), 8, BLACK);
    display.fillRect(6 * 7 + 6 * (editing) + 4, 10 * 2 + 13, 7, 8, WHITE); //set cursor
    for (uint8_t i = 0; i < 10; i++) {
      display.setTextColor(i == editing ? BLACK : WHITE);
      display.print(call_sign[i]);
    }
  }
  else {
    display.print(call_sign);
  }
 
  display.setTextColor(menu_id == 3 ? BLACK : WHITE);
  display.setCursor(5, 10 * 4 + 3);
  display.print(PSTR2("CALIBRATE RSSI"));
 
  display.setTextColor(menu_id == 4 ? BLACK : WHITE);
  display.setCursor(5, 10 * 5 + 3);
  display.print(PSTR2("SAVE & EXIT"));
  display.display();
}
 
void screens::save(uint8_t mode, uint8_t channelIndex, uint16_t channelFrequency, const char *call_sign,int lfav)
{
  reset();
  drawTitleBox(PSTR2("SAVE SETTINGS"));
 
  display.setTextColor(WHITE);
  display.setCursor(5, 8 * 1 + 4);
   display.print(PSTR2("SETTINGS SAVED"));
//  display.setCursor(38, 8 * 1 + 4);
//  switch (mode)
//  {
//    case STATE_SCAN: // Band Scanner
//      display.print(PSTR2("BAND SCANNER"));
//      break;
//    case STATE_MANUAL: // manual mode
//      display.print(PSTR2("MANUAL"));
//      break;
//    case STATE_FAVORITE: // FAVORITE mode
//      display.print(PSTR2("FAVORITE"));
//      break;
//    case STATE_SEEK: // seek mode
//      display.print(PSTR2("AUTO SEEK"));
//      break;
//  }
// 
//  display.setCursor(5, 8 * 2 + 4);
//  display.print(PSTR2("BAND:"));
//  display.setCursor(38, 8 * 2 + 4);
//  // print band
//#ifdef USE_LBAND
//  if (channelIndex > 39)
//  {
//    display.print(PSTR2("D/5.3    "));
//  }
//  else if (channelIndex > 31)
//#else
//  if (channelIndex > 31)
//#endif
//  {
//    display.print(PSTR2("C/Race"));
//  }
//  else if (channelIndex > 23)
//  {
//    display.print(PSTR2("F/Airwave"));
//  }
//  else if (channelIndex > 15)
//  {
//    display.print(PSTR2("E"));
//  }
//  else if (channelIndex > 7)
//  {
//    display.print(PSTR2("B"));
//  }
//  else
//  {
//    display.print(PSTR2("A"));
//  }
 
  display.setCursor(5, 8 * 3 + 4);
  display.print(PSTR2("CHAN:"));
  display.setCursor(38, 8 * 3 + 4);
  uint8_t active_channel = channelIndex % CHANNEL_BAND_SIZE + 1; // get channel inside band
  display.print(active_channel, DEC);
  if (lfav!=-99)
  {
  display.print(PSTR2("  FAV CHAN:"));
  display.print(lfav);
  }
  display.setCursor(5, 8 * 4 + 4);
  display.print(PSTR2("FREQ:     MHz"));
  display.setCursor(38, 8 * 4 + 4);
  display.print(channelFrequency);
// 
//  display.setCursor(5, 8 * 5 + 4);
//  display.print(PSTR2("SIGN:"));
//  display.setCursor(38, 8 * 5 + 4);
//  display.print(call_sign);
// 
  display.setCursor(((display.width() - 11 * 6) / 2), 8 * 6 + 4);
  display.print(PSTR2("-- SAVED --"));
  display.display();
}
 
void screens::updateSave(const char * msg)
{
  display.setTextColor(WHITE, BLACK);
  display.setCursor(((display.width() - strlen(msg) * 6) / 2), 8 * 6 + 4);
  display.print(msg);
  display.display();
}
 
 
#endif
