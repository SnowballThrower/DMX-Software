#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "Utils.h"

void loadValues() {
  deactivateFaders();
  int i;
  for (i = 0; i < ChN; i++) {
    values[i] = EEPROM.read(CHSTART + i);
  }
}

void saveValues() {
  int i;
  for (i = 0; i < ChN; i++) {
    if (values[i] != EEPROM.read(CHSTART + i)) {
      EEPROM.write(CHSTART + i, values[i]);
    }
  }
}
