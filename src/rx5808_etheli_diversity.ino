//This '.ino' file is a stub to support building via the original
// Arduino IDE.  The real code is in the '.cpp' and '.h' files.
//
//The developer-preferred way of editing and building the project
//is via Eclipse and the "Eclipse C++ IDE for Arduino" plugin:
// https://marketplace.eclipse.org/content/eclipse-c-ide-arduino




// arduino 1.8.13 compile ok; 

// arduino IDE, ProMini, 328P 5V 16MHz

// upload via shell avrdude



// or can build in new eclipse+arduino-support


// v1.HH	- start in scan mode with receiver B only
//			- scan mode: use more screen height for graph
//			- track raw rssi values
// v1.1		- scan by frequency range (preparation)
//			- have min/max range tunable via up/down buttons
// v1.2		- scan by frequency range
//			- add ticker @ 50MHz
//			- add level @ half of best rssi
// v1.3		- improve display
//				- take 120 samples, not just 40, but accept the slowdown
//				- more screen estate for bargraph: only one bottom line of text
// v1.3c	- better removal of texts
//			- center bestFreq text around the bestFreq mark
// v1.3d	- remember best.rssi.a/b from previous cycle
//

