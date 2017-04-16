#include <LiquidCrystal.h>
#include "Status.h"
#include "Utils.h"


int mode = 0;

char Name[16]; // buffer
byte MenuPosition = 0; // actual position
bool in = true; // navigating inside menu


const char MenuPoint_0[] PROGMEM = "      Exit      ";
const char MenuPoint_1[] PROGMEM = "  Simple Mode   ";
const char MenuPoint_2[] PROGMEM = "    Channel     ";
const char MenuPoint_3[] PROGMEM = "     Fader      ";
const char MenuPoint_4[] PROGMEM = "    Settings    ";
const char MenuPoint_5[] PROGMEM = "     Colour     ";
const char MenuPoint_6[] PROGMEM = "Flow(1Spd,2-4Of)";
const char MenuPoint_7[] PROGMEM = "      Dimm      ";
const char MenuPoint_8[] PROGMEM = "     Value      ";
const char MenuPoint_9[] PROGMEM = "   FlowEnable   ";
const char MenuPoint_10[] PROGMEM = " StandardGlobal ";
const char MenuPoint_11[] PROGMEM = "     Offset     ";
const char MenuPoint_12[] PROGMEM = " FixedChannnel  ";
const char MenuPoint_13[] PROGMEM = " GlobalChannnel ";
const char MenuPoint_14[] PROGMEM = "     Speed      ";
const char MenuPoint_15[] PROGMEM = "ChannelDistance ";
const char MenuPoint_16[] PROGMEM = "   Global End   ";
const char MenuPoint_17[] PROGMEM = "  Global Start  ";
const char MenuPoint_18[] PROGMEM = "  Flow1 Start   ";
const char MenuPoint_19[] PROGMEM = "   Flow1 End    ";
const char MenuPoint_20[] PROGMEM = "  Flow2 Start   ";
const char MenuPoint_21[] PROGMEM = "   Flow2 End    ";
const char MenuPoint_22[] PROGMEM = "  MIDI enable   ";

const char* const MenuPoints[] PROGMEM = {MenuPoint_0, MenuPoint_1, MenuPoint_2, MenuPoint_3, MenuPoint_4, MenuPoint_5, MenuPoint_6, MenuPoint_7, MenuPoint_8, MenuPoint_9, MenuPoint_10, MenuPoint_11, MenuPoint_12, MenuPoint_13, MenuPoint_14, MenuPoint_15, MenuPoint_16, MenuPoint_17, MenuPoint_18, MenuPoint_19, MenuPoint_20, MenuPoint_21, MenuPoint_22};

bool Leaf[]       = {false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
//                 Ex, Si, Ch, Fa, Se, Co, Fl, Di, Va, Fl, St, Of, Fi, Gl, Sp, Ch, Gl, Gl, Fl, Fl, Fl, Fl, MI
//                 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
byte MenuJump[] = {1 , 5 , 8 , 11, 14, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
byte MenuMax[] =  {4 , 4 , 4 , 4 , 4 , 7 , 7 , 7 , 10, 10, 10, 13, 13, 13, 22, 22, 22, 22, 22, 22, 22, 22, 22};
byte MenuMin[] =  {1 , 1 , 1 , 1 , 1 , 5 , 5 , 5 , 8 , 8 , 8 , 11, 11, 11, 14, 14, 14, 14, 14, 14, 14, 14, 14};
byte MenuFunc[] = {0 , 0 , 0 , 0 , 0 , 31, 32, 33, 4 , 5 , 6 , 7 , 8 , 9 , 11, 12, 13, 14, 15, 16, 17, 18, 19};
byte BackJump[] = {0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 2 , 2 , 2 , 3 , 3 , 3 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 };





//*********************************************

String MenuName(int m) { // getting Name of MenuPoint
  strcpy_P(Name, (char*)pgm_read_word(&(MenuPoints[m])));
  return Name;
}

void MenuLeft() { // navigate to left
  MenuPosition = sub(MenuPosition, MenuMin[MenuPosition], MenuMax[MenuPosition]);
}

void MenuRight() { // navigate to right
  MenuPosition = add(MenuPosition, MenuMin[MenuPosition], MenuMax[MenuPosition]);
}

void MenuClick() { // accept MenuPoint -> jump to other point or switch to function
  if (Leaf[MenuPosition]) {
    in = false;
  } else {
    MenuPosition = MenuJump[MenuPosition];
  }
}

void MenuBack() { // jump back
  MenuPosition = BackJump[MenuPosition];
}

void MenuBackRight() { // to next menu-region
  MenuBack();
  MenuRight();
  MenuClick();
  in = true;
}



void MenuStart() { // to first menu-region
  MenuPosition = 0;
  MenuRight();
  MenuClick();
  in = true;
}

void printMenu() {
  lcd.setCursor(0, 0);
  lcd.print(MenuName(MenuPosition));
}

void menu() {
  if (pf[Set]) {
    MenuBackRight();
    pf[Set] = false;
    printMenu();
  }
  if (pf[Men]) {
    MenuRight();
    pf[Men] = false;
    printMenu();
  }
}
