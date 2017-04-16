#ifndef devices
#define devices

#include <Arduino.h>
#include "Globals.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>


//Memory

char names[(ChN + xCh) * 2];
char deviceNames[noD * 2];
byte typeLength[noD];
int deviceStart[noD];
byte deviceType[noD];
char channelNames[(ChN + xCh) * 2];
char typeNames[noT * 3];
char typeChnNames[maxCh  * 2];
byte smF[maxCh * noT]; //simpleMode
byte smW[maxCh * noT]; //simpleMode

int twoBytes;
byte lByte;
byte hByte;




char getTypeName(int type, int ziffer) {
  return EEPROM.read(TYPSTART + tpB * type + ziffer + 1);
}
int getTypeLength(int type) {
  return EEPROM.read(TYPSTART + tpB * type);
}
void readTypeChannelNames(int type) {
  int i;
  for (i = 0; i < maxCh; i++) {
    typeChnNames[2 * i] =      EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i);
    typeChnNames[2 * i + 1] =  EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i + 1);
  }
}

void loadTypes(int type) {
  typeLength[type] = EEPROM.read(TYPSTART + tpB * type);
  typeNames[type * 3 + 0] = EEPROM.read(TYPSTART + tpB * type + 1);
  typeNames[type * 3 + 1] = EEPROM.read(TYPSTART + tpB * type + 2);
  typeNames[type * 3 + 2] = EEPROM.read(TYPSTART + tpB * type + 3);
  int i;
  for (i = 0; i < typeLength[type]; i++) {
    smF[type * maxCh + i] =                       EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 2);
    smW[type * maxCh + i] =                       EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 3);
  }

}
char getChannelName(int type, int channel, int ziffer, int i) {
  return EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i + ziffer);
}
/*
void loadType(int type) {
  typeLength[type] = EEPROM.read(TYPSTART + tpB * type);
  typeNames[type * 3 + 0] = EEPROM.read(TYPSTART + tpB * type + 1);
  typeNames[type * 3 + 1] = EEPROM.read(TYPSTART + tpB * type + 2);
  typeNames[type * 3 + 2] = EEPROM.read(TYPSTART + tpB * type + 3);
  for (i = 0; i < typeLength[type]; i++) {
    typeChnNames[type * 2 * maxCh + 2 * i] =      EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i);
    typeChnNames[type * 2 * maxCh + 2 * i + 1] =  EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i + 1);
    smF[type * maxCh + i] =                       EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 2);
    smW[type * maxCh + i] =                       EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 3);
  }

}
*/
void saveType(int type) {
  //lcd.setCursor(0,0);
  //lcd.print(String(type));
  if (typeLength[type] != EEPROM.read(TYPSTART + tpB * type)) {
    EEPROM.write(TYPSTART + tpB * type, typeLength[type]);
  }
  if (typeNames[type * 3 + 0] != EEPROM.read(TYPSTART + tpB * type + 1)) {
    EEPROM.write(TYPSTART + tpB * type + 1, typeNames[type * 3 + 0]);
  }
  if (typeNames[type * 3 + 1] != EEPROM.read(TYPSTART + tpB * type + 2)) {
    EEPROM.write(TYPSTART + tpB * type + 2, typeNames[type * 3 + 1]);
  }
  if (typeNames[type * 3 + 2] != EEPROM.read(TYPSTART + tpB * type + 3)) {
    EEPROM.write(TYPSTART + tpB * type + 3, typeNames[type * 3 + 2]);
  }
  int i;
  for (i = 0; i < typeLength[type]; i++) {
    /*
    if (typeChnNames[type * 2 * maxCh + 2 * i] !=     EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i)) {
      EEPROM.write(NAMESTART + type * tpN * maxCh + tpN * i,      typeChnNames[type * 2 * maxCh + 2 * i]);
    }
    if (typeChnNames[type * 2 * maxCh + 2 * i + 1] != EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i + 1)) {
      EEPROM.write(NAMESTART + type * tpN * maxCh + tpN * i + 1,  typeChnNames[type * 2 * maxCh + 2 * i + 1]);
    }
    */
    if (smF[type * maxCh + i] !=                      EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 2)) {
      EEPROM.write(NAMESTART + tpN * type * maxCh + tpN * i + 2, smF[type * maxCh + i]);
    }
    if (smW[type * maxCh + i] !=                      EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 3)) {
      EEPROM.write(NAMESTART + tpN * type * maxCh + tpN * i + 3, smW[type * maxCh + i]);
    }
  }
}

void save1Type(int type) {
  int i;
  //lcd.setCursor(0,0);
  //lcd.print(String(type));
  if (typeLength[type] != EEPROM.read(TYPSTART + tpB * type)) {
    EEPROM.write(TYPSTART + tpB * type, typeLength[type]);
  }
  if (typeNames[type * 3 + 0] != EEPROM.read(TYPSTART + tpB * type + 1)) {
    EEPROM.write(TYPSTART + tpB * type + 1, typeNames[type * 3 + 0]);
  }
  if (typeNames[type * 3 + 1] != EEPROM.read(TYPSTART + tpB * type + 2)) {
    EEPROM.write(TYPSTART + tpB * type + 2, typeNames[type * 3 + 1]);
  }
  if (typeNames[type * 3 + 2] != EEPROM.read(TYPSTART + tpB * type + 3)) {
    EEPROM.write(TYPSTART + tpB * type + 3, typeNames[type * 3 + 2]);
  }
  for (i = 0; i < typeLength[type]; i++) {
    if (typeChnNames[2 * i] !=     EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i)) {
      EEPROM.write(NAMESTART + type * tpN * maxCh + tpN * i,      typeChnNames[2 * i]);
    }
    if (typeChnNames[2 * i + 1] != EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i + 1)) {
      EEPROM.write(NAMESTART + type * tpN * maxCh + tpN * i + 1,  typeChnNames[2 * i + 1]);
    }
    if (smF[type * maxCh + i] !=                      EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 2)) {
      EEPROM.write(NAMESTART + tpN * type * maxCh + tpN * i + 2, smF[type * maxCh + i]);
    }
    if (smW[type * maxCh + i] !=                      EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 3)) {
      EEPROM.write(NAMESTART + tpN * type * maxCh + tpN * i + 3, smW[type * maxCh + i]);
    }
  }

}

void loadDevice(int dev) {
  deviceType[dev] = EEPROM.read(DEVSTART + tpD * dev);

  lByte = EEPROM.read(DEVSTART + tpD * dev + 1);
  hByte = EEPROM.read(DEVSTART + tpD * dev + 2);
  twoBytes = 256 * lByte + hByte;
  deviceStart[dev] = twoBytes;
  deviceNames[dev * 2] = EEPROM.read(DEVSTART + tpD * dev + 3);
  deviceNames[dev * 2 + 1] = EEPROM.read(DEVSTART + tpD * dev + 4);
}
void saveDevice(int dev) {
  if (deviceType[dev] != EEPROM.read(DEVSTART + tpD * dev)) {
    EEPROM.write(DEVSTART + tpD * dev, deviceType[dev]);
  }
  lByte = EEPROM.read(DEVSTART + tpD * dev + 1);
  hByte = EEPROM.read(DEVSTART + tpD * dev + 2);
  twoBytes = 256 * lByte + hByte;
  if (deviceStart[dev] != twoBytes) {
    twoBytes = deviceStart[dev];
    lByte = twoBytes / 256;
    hByte = twoBytes % 256;
    EEPROM.write(DEVSTART + tpD * dev + 1, lByte);
    EEPROM.write(DEVSTART + tpD * dev + 2, hByte);
  }
  if (deviceNames[dev * 2] != EEPROM.read(DEVSTART + tpD * dev + 3)) {
    EEPROM.write(DEVSTART + tpD * dev + 3, deviceNames[dev * 2]);
  }
  if (deviceNames[dev * 2 + 1] != EEPROM.read(DEVSTART + tpD * dev + 4)) {
    EEPROM.write(DEVSTART + tpD * dev + 4, deviceNames[dev * 2 + 1]);
  }
}

void loadDevTyp() {
  int i;
  for (i = 0; i < noT; i++) {
    loadTypes(i);
  }
  for (i = 0; i < noD; i++) {
    loadDevice(i);
  }
}


void setupTypes() {
  int i;
  for (i = 0; i < noT * maxCh; i++) {
    smF[i] = 0;
    smW[i] = 0;
  }
  readTypeChannelNames(0);
  int tp = 1;   //ADJ
  typeLength[tp] = 7;
  typeNames[tp * 3 + 0] = 'A';
  typeNames[tp * 3 + 1] = 'D';
  typeNames[tp * 3 + 2] = 'J';
  typeChnNames[0] = 'R';
  typeChnNames[1] = 'o';
  typeChnNames[2] = 'G';
  typeChnNames[3] = 'r';
  typeChnNames[4] = 'B';
  typeChnNames[5] = 'l';
  typeChnNames[6] = 'M';
  typeChnNames[7] = 'a';
  typeChnNames[8] = 'S';
  typeChnNames[9] = 'p';
  typeChnNames[10] = 'M';
  typeChnNames[11] = 'o';
  typeChnNames[12] = 'D';
  typeChnNames[13] = 'm';
  smF[tp * maxCh + 3] = 2;
  smF[tp * maxCh + 6] = 1;
  save1Type(tp);

  readTypeChannelNames(0);
  tp = 2;   //Renkforce
  typeLength[tp] = 10;
  typeNames[tp * 3 + 0] = 'R';
  typeNames[tp * 3 + 1] = 'f';
  typeNames[tp * 3 + 2] = 'P';
  typeChnNames[0] = 'D';
  typeChnNames[1] = 'm';
  typeChnNames[2] = 'R';
  typeChnNames[3] = 'o';
  typeChnNames[4] = 'G';
  typeChnNames[5] = 'r';
  typeChnNames[6] = 'B';
  typeChnNames[7] = 'l';
  typeChnNames[8] = 'W';
  typeChnNames[9] = 'e';
  typeChnNames[10] = 'A';
  typeChnNames[11] = 'm';
  typeChnNames[12] = 'S';
  typeChnNames[13] = 'r';
  typeChnNames[14] = 'M';
  typeChnNames[15] = 'a';
  typeChnNames[16] = 'M';
  typeChnNames[17] = 's';
  typeChnNames[18] = 'D';
  typeChnNames[19] = 'c';
  smF[tp * maxCh + 0] = 3;
  smF[tp * maxCh + 7] = 4;
  save1Type(tp);

  readTypeChannelNames(0);
  tp = 3;   //Cameo
  typeLength[tp] = 9;
  typeNames[tp * 3 + 0] = 'C';
  typeNames[tp * 3 + 1] = 'a';
  typeNames[tp * 3 + 2] = 'P';
  typeChnNames[0] = 'D';
  typeChnNames[1] = 'm';
  typeChnNames[2] = 'S';
  typeChnNames[3] = 'r';
  typeChnNames[4] = 'R';
  typeChnNames[5] = 'o';
  typeChnNames[6] = 'G';
  typeChnNames[7] = 'r';
  typeChnNames[8] = 'B';
  typeChnNames[9] = 'l';
  typeChnNames[10] = 'W';
  typeChnNames[11] = 'e';
  typeChnNames[12] = 'A';
  typeChnNames[13] = 'm';
  typeChnNames[14] = 'U';
  typeChnNames[15] = 'V';
  typeChnNames[16] = 'M';
  typeChnNames[17] = 'a';
  smF[tp * maxCh + 0] = 5;
  smF[tp * maxCh + 8] = 6;
  save1Type(tp);

  readTypeChannelNames(0);
  tp = 4;   //Bar24
  typeLength[tp] = 24;
  typeNames[tp * 3 + 0] = 'B';
  typeNames[tp * 3 + 1] = 'a';
  typeNames[tp * 3 + 2] = 'r';
  for (i = 0; i < 4; i++) { //nur gültig für maxCh = 12!
    typeChnNames[i * 6 + 0] = 'R';
    typeChnNames[i * 6 + 1] = (char) i + 1 + 48;
    typeChnNames[i * 6 + 2] = 'G';
    typeChnNames[i * 6 + 3] = (char) i + 1 + 48;
    typeChnNames[i * 6 + 4] = 'B';
    typeChnNames[i * 6 + 5] = (char) i + 1 + 48;
  }
  save1Type(tp);

  readTypeChannelNames(0);
  tp = 5;   //Bar24 forts.
  typeLength[tp] = 12;
  typeNames[tp * 3 + 0] = '1';
  typeNames[tp * 3 + 1] = 'B';
  typeNames[tp * 3 + 2] = 'a';
  for (i = 0; i < 4; i++) { //nur gültig für maxCh = 12!
    typeChnNames[i * 6 + 0] = 'R';
    typeChnNames[i * 6 + 1] = (char) i + 1 + 4 + 48;
    typeChnNames[i * 6 + 2] = 'G';
    typeChnNames[i * 6 + 3] = (char) i + 1 + 4 + 48;
    typeChnNames[i * 6 + 4] = 'B';
    typeChnNames[i * 6 + 5] = (char) i + 1 + 4 + 48;
  }
  save1Type(tp);

  //6 reserviert für Bar24 forts.

  readTypeChannelNames(0);
  tp = 7;   //Bar5
  typeLength[tp] = 5;
  typeNames[tp * 3 + 0] = 'B';
  typeNames[tp * 3 + 1] = 'r';
  typeNames[tp * 3 + 2] = '5';
  typeChnNames[0] = 'R';
  typeChnNames[1] = 'o';
  typeChnNames[2] = 'G';
  typeChnNames[3] = 'r';
  typeChnNames[4] = 'B';
  typeChnNames[5] = 'l';
  typeChnNames[6] = 'D';
  typeChnNames[7] = 'm';
  typeChnNames[8] = 'S';
  typeChnNames[9] = 'r';
  smF[tp * maxCh + 3] = 7;
  smF[tp * maxCh + 0] = 8;
  smF[tp * maxCh + 1] = 8;
  smF[tp * maxCh + 2] = 8;
  save1Type(tp);

  readTypeChannelNames(0);
  tp = 8;   //Outdoor
  typeLength[tp] = 7;
  typeNames[tp * 3 + 0] = 'O';
  typeNames[tp * 3 + 1] = 'u';
  typeNames[tp * 3 + 2] = 't';
  typeChnNames[0] = 'D';
  typeChnNames[1] = 'm';
  typeChnNames[2] = 'R';
  typeChnNames[3] = 'o';
  typeChnNames[4] = 'G';
  typeChnNames[5] = 'r';
  typeChnNames[6] = 'B';
  typeChnNames[7] = 'l';
  typeChnNames[8] = 'W';
  typeChnNames[9] = 'e';
  typeChnNames[10] = 'S';
  typeChnNames[11] = 'r';
  typeChnNames[12] = 'P';
  typeChnNames[13] = 'r';
  save1Type(tp);

  readTypeChannelNames(0);
  tp = noT - 1;
  typeLength[tp] = xCh;
  typeNames[tp * 3 + 0] = 'D';
  typeNames[tp * 3 + 1] = 'i';
  typeNames[tp * 3 + 2] = 's';
  typeChnNames[0] = 'R';
  typeChnNames[1] = 'o';
  typeChnNames[2] = 'G';
  typeChnNames[3] = 'r';
  typeChnNames[4] = 'B';
  typeChnNames[5] = 'l';
  typeChnNames[6] = 'D';
  typeChnNames[7] = 'm';
  typeChnNames[8] = 'C';
  typeChnNames[9] = 'o';
  smW[tp * maxCh + 0] = 255;
  smW[tp * maxCh + 1] = 255;
  smW[tp * maxCh + 2] = 0;
  smW[tp * maxCh + 3] = 255;
  smW[tp * maxCh + 4] = 55;
  save1Type(tp);

  readTypeChannelNames(0);
}
void setupChannels() {
  int i;
  int k;
  for (k = 0; k < noD * 2; k++) {
    deviceNames[k] = '|';
  }
  for (k = 0; k < noD; k++) {
    deviceStart[k] = 0;
  }
  for (k = 0; k < noT; k++) {
    typeLength[k] = 0;
  }
  for (i = 0; i < (ChN + xCh) * 2; i++) {
    names[i] = '|';
    channelNames[i] = '-';
  }

  loadDevTyp();

  for (k = 0; k < noD; k++) {
    readTypeChannelNames(deviceType[k]);
    for (i = deviceStart[k]; i < (deviceStart[k] + typeLength[deviceType[k]]); i++) {
      names[i * 2] = deviceNames[k * 2];
      names[i * 2 + 1] = deviceNames[k * 2 + 1];
      channelNames[i * 2] = typeChnNames[(i - deviceStart[k]) * 2];
      channelNames[i * 2 + 1] = typeChnNames[(i - deviceStart[k]) * 2 + 1];
    }
  }

}

void setupNames() {
  int i;
  int k;
  for (k = 0; k < noD * 2; k++) {
    deviceNames[k] = '|';
  }
  for (k = 0; k < noD; k++) {
    deviceStart[k] = 0;
  }
  for (k = 0; k < noT; k++) {
    typeLength[k] = 0;
  }



  deviceNames[0] = 'A';
  deviceNames[1] = 'D';
  deviceNames[2] = 'C';
  deviceNames[3] = '1';
  deviceNames[4] = 'O';
  deviceNames[5] = '1';
  deviceNames[6] = 'O';
  deviceNames[7] = '2';
  deviceNames[8] = 'R';
  deviceNames[9] = '1';
  deviceNames[10] = 'R';
  deviceNames[11] = '2';

  deviceNames[20] = 'B';
  deviceNames[21] = '1';
  deviceNames[22] = 'B';
  deviceNames[23] = '1';
  deviceNames[24] = 'B';
  deviceNames[25] = '2';
  deviceNames[26] = 'B';
  deviceNames[27] = '2';
  deviceNames[28] = 'B';
  deviceNames[29] = '3';
  deviceNames[30] = 'B';
  deviceNames[31] = '3';
  deviceNames[32] = 'B';
  deviceNames[33] = '4';
  deviceNames[34] = 'B';
  deviceNames[35] = '4';

  deviceNames[noD * 2 - 2] = 'D';
  deviceNames[noD * 2 - 1] = 'i';



  deviceType[0] = 1;  //ADJ
  deviceType[1] = 3;  //Cameo
  deviceType[2] = 8;  //Outdoor
  deviceType[3] = 8;  //Outdoor
  deviceType[4] = 2;  //Renkforce
  deviceType[5] = 2;  //Renkforce

  deviceType[10] = 4;  //Bar 1
  deviceType[11] = 5;  //Bar forts.
  deviceType[12] = 4;  //Bar 2
  deviceType[13] = 5;  //Bar forts.
  deviceType[14] = 4;  //Bar 3
  deviceType[15] = 5;  //Bar forts.
  deviceType[16] = 4;  //Bar 4
  deviceType[17] = 5;  //Bar forts.

  deviceType[noD - 1] = noT - 1;



  deviceStart[0] = 1 - 1; //ADJ
  deviceStart[1] = 10 - 1; //Cameo
  deviceStart[2] = 20 - 1; //Outdoor
  deviceStart[3] = 30 - 1; //Outdoor
  deviceStart[4] = 40 - 1; //Renkforce
  deviceStart[5] = 50 - 1; //Renkforce

  deviceStart[10] = 100 - 1; //Bar
  deviceStart[11] = 100 + maxCh - 1; //Bar
  deviceStart[12] = 124 - 1; //Bar
  deviceStart[13] = 124 + maxCh - 1; //Bar
  deviceStart[14] = 148 - 1; //Bar
  deviceStart[15] = 148 + maxCh - 1; //Bar
  deviceStart[16] = 172 - 1; //Bar
  deviceStart[17] = 172 + maxCh - 1; //Bar

  deviceStart[noD - 1] = ChN;


  for (i = 0; i < (ChN + xCh) * 2; i++) {
    names[i] = '|';
    channelNames[i] = '-';
  }


  setupTypes();


  for (k = 0; k < noD; k++) {
    readTypeChannelNames(k);
    for (i = deviceStart[k]; i < (deviceStart[k] + typeLength[deviceType[k]]); i++) {
      names[i * 2] = deviceNames[k * 2];
      names[i * 2 + 1] = deviceNames[k * 2 + 1];
      channelNames[i * 2] = typeChnNames[(i - deviceStart[k]) * 2];
      channelNames[i * 2 + 1] = typeChnNames[(i - deviceStart[k]) * 2 + 1];
    }
  }


  for (i = 0; i < noD; i++) {
    saveDevice(i);
  }
  for (i = 0; i < noT; i++) {
    saveType(i);
  }

  EEPROM.write(FLASHED, flashNumber);
}


#endif
