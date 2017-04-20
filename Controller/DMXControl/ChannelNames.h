#ifndef chNames
#define chNames

#include <avr/pgmspace.h>

const char Ch_0[] PROGMEM = "     None       ";
const char Ch_1[] PROGMEM = "      Red       ";
const char Ch_2[] PROGMEM = "     Green      ";
const char Ch_3[] PROGMEM = "      Blue      ";
const char Ch_4[] PROGMEM = "     White      ";
const char Ch_5[] PROGMEM = "     Amber      ";
const char Ch_6[] PROGMEM = "  Ultra-Violet  ";
const char Ch_7[] PROGMEM = "  Color-Makro   ";
const char Ch_8[] PROGMEM = "     Dimmer     ";
const char Ch_9[] PROGMEM = "     Strobe     ";
const char Ch_10[] PROGMEM = "     Program    ";
const char Ch_11[] PROGMEM = "   Undefined 0  ";
const char Ch_12[] PROGMEM = "   Undefined 1  ";

const char* const Chs[] PROGMEM = {Ch_0, Ch_1, Ch_2, Ch_3, Ch_4, Ch_5, Ch_6, Ch_7, Ch_8, Ch_9, Ch_10, Ch_11};

#endif
