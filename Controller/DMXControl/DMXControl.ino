#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "Globals.h"
#include "Devices.h"
#include "Menu.h"
#include "Utils.h"
#include "Scenes.h"
#include "Status.h"
#include "Midi.h"



int pc6 = 22; //unused
int pc7 = 23; //unused


//Fader
int As[] = {A0, A1, A2, A3, A4, A5, A6, A7};

//*************************************************



//const int maxCh = 12; //12 damit passend für Bar!
//const int noD = 33;
//const int noT = 20;
//char deviceNames[noD * 2];
//byte typeLength[noD];
//int deviceStart[noD];
//byte deviceType[noD];
//char channelNames[(ChN + xCh) * 2];
//char typeNames[noT * 3];
//char typeChnNames[maxCh  * 2];
//byte smF[maxCh * noT];
//byte smW[maxCh * noT];

//int FLASHED = 2000;
//int flashNumber = 176;
//int CHSTART = 0;
//int TYPSTART = 1000;
//int NAMESTART = 1100;
//int DEVSTART = 1800;
//int tpB = 4;
//int tpD = 5;
//int tpN = 4;



int ChD1 = 7;
int ChD2 = 10;
int G1S = 0;
int G1E = ChD1 * 4;
int G2S = ChD1 * 4;
int G2E = ChD1 * 4 + ChD2 * 4;

int spd = 20;
int Fl1S = 0;
int Fl1E = 4 * 7;
int Fl2S = ChN;
int Fl2E = ChN + xCh;
bool up[ChN + xCh];

byte valuebuffer;



bool Up2 = true;
int selected = 3;

bool lower = false;
bool turn = false;
bool turnRnL = false;
bool push = false;
bool pbutton = false;


//*********************
//constants
int diff = 6;
int Min = 10;
int Max = 1018;
double dTime = 0; //0.1
double rTime = 1.0;
int transSteps = 32;
//*********************
//counters
int t = 0; //transmit
int s = 0;
int c = 0; //u.a. simpleFader
int d = 0; //simpleFader
int p; //displayNames
int x; //transmit
int z = 0; //flow
int i = 0;
int k = 0;
//int twoBytes;
//byte lByte;
//byte hByte;
int schritt = 0;
int wdh = 0;
double sendingTime = 0;
int g; //targetChannel
//*********************
//inits
byte dr = 0;
byte dg = 0;
byte db = 0;
byte dd = 0;
byte dc = 110;

int Ch = 0;
int dev = 0;
bool DevChn = true;
//*********************
// simpleMode
byte smV[ChN + xCh];
const int noP = 1;
int page = 0;
char faderNames[9 * 16 * noP];

//*************************************************

//*********************************************
//
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

  pinMode(l, INPUT);
  pinMode(r, INPUT);
  pinMode(sB, INPUT);
  pinMode(fB, INPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(sLED, OUTPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);


  digitalWrite(sLED, HIGH);
  //startProz++;
  select(startProz);
  for (i = 0; i < ChN + xCh; i++) {
    values[i] = 0;
  }
  values[ChN + 0] = 255;
  values[ChN + 1] = 255;
  values[ChN + 2] = 0;
  values[ChN + 3] = 255;
  values[ChN + 4] = 60;
  for (i = 0; i < 8; i++) {
    active[i] = false;
    ofs[i] = i;
    fix[i] = false;
    fader[i] = analogRead(As[i]);
    fadeOld[i] = fader[i];
  }
  MenuPosition = 4;//MenuStart();
  setupFaderNames();

  digitalWrite(sLED, HIGH);
  //startProz++;
  select(startProz);

  if (EEPROM.read(FLASHED) != flashNumber) {
    setupNames();
  }

  digitalWrite(sLED, HIGH);
  startProz++;
  select(startProz);

  setupChannels();

  digitalWrite(sLED, HIGH);
  startProz++;
  select(startProz);
  setupFlows();

  schritt = 0;
  wdh = 0;
  //analogWrite(R,values[0]);
  //analogWrite(G,values[1]);
  //analogWrite(B,values[2]);
  //analogWrite(C,255);
  //digitalWrite(sLED,HIGH);
  Serial.begin(31250);
  attachInterrupt(l, rotLeft, RISING);
  //attachInterrupt(r, rotRight, RISING);

  lcd.clear();
  lcd.print(String(Ch + 1));
  z = 0;

  lcd.setCursor(0, 1);
  for (i = 0; i < 8; i++) {
    lcd.print(names[2 * targetChannel(i)]);
    lcd.print(names[2 * targetChannel(i) + 1]);
  }
  s = 0;
  //printMenu();
  for (p = 0; p < 8; p++) {
    printChannelName(p);
  }
  mode = 1;//simpleInit();
}

//*************************************************

void loop() {
  select(s);
  if (mode != 3) {
    digitalWrite(sLED, active[s]);
  }
  switch (mode) {
    case 0: simpleLoop(); break;
    case 1:
    case 2: channelLoop(); break;
    case 3: analogWrite(sLED, led[s]); remoteLoop(); break;
    default: mode = 0; simpleInit(); break;
  }
  s++;
  if (s > 7) {
    s = 0;
  }
  digitalWrite(sLED, LOW);
}
void changeMode() {
  deactivateFaders();
  switch (mode) {
    case 0: simpleInit; break;
    case 1:
    case 2: for (p = 0; p < 8; p++) {
        printChannelName(p);
      } break;
    case 3: lcd.clear(); lcd.print("Remote"); break;
    default: break;
  }
}
void channelLoop() {



  encoder();
  if (pf[Enc]) {
    DevChn = !DevChn;
    pf[Enc] = false;
    lcd.clear();
    for (p = 0; p < 8; p++) {
      printChannelName(p);
    }
  }
  if (pf[loa]) {
    loadValues();
    pf[loa] = false;
  }
  if (pf[sav]) {
    saveValues();
    pf[sav] = false;
  }
  menu();

  delay(dTime);
  transmitter();
  buttonRead(s);

  midiActive = pbf[GlA];

  flow();

  valueRead();

  displayAnalog();

}


void remoteLoop() {

  encoder();

  transmitter();

  delay(rTime);
  Serial1.flush();
  buttonRead(s);

  midiActive = true;

  valueReadChange();

  displayAnalog();

}

//****************************************************



void buttonRead(int s) {
  pbp = digitalRead(powerButton) == 1;
  pbs[s] = digitalRead(sB) == 0;
  pbf[s] = digitalRead(fB) == 1;
  if (pbp && !pushp) {
    pushp = true;
    //pp = true;
    mode++;
    changeMode();
  } else {
    pushp = pbp;
  }

  if (mode == 3) {
    if (!pbs[s] && pushs[s]) {
      midiButtonSend(false, false, s);
    }
  }
  if (pbs[s] && !pushs[s]) {
    pushs[s] = true;
    if (mode == 3) {
      midiButtonSend(false, true, s);
    }
  } else {
    pushs[s] = pbs[s];
  }
  if (mode == 3) {
    if (!pbf[s] && pushf[s]) {
      midiButtonSend(true, false, s);
    }
  }
  if (pbf[s] && !pushf[s]) {
    pushf[s] = true;
    pf[s] = true;
    if (mode == 3) {
      midiButtonSend(true, true, s);
    }
  } else {
    pushf[s] = pbf[s];
  }

}
void transmitter() {
  Serial1.flush();
  transmit(t * 512 / transSteps);
  t++;
  if (t >= transSteps) {
    t = 0;
  }
}
void transmitter2() {
  Serial1.flush();
  transmit2(t * 512 / transSteps);
  t++;
  if (t >= transSteps) {
    t = 0;
  }
}
void transmit(int s) {
  if (s == 0) {
    Serial1.begin(10000, SERIAL_8N2);
    Serial1.write(255);
    Serial1.begin(250000, SERIAL_8N2);
    Serial1.write(0);
  }
  x = s;
  while (x < s + (512 / transSteps)) {

    Serial1.write(values[x]);
    x++;
  }
}
void transmit2(int s) {
  if (s == 0) {
    Serial1.begin(10000, SERIAL_8N2);
    Serial1.write(255);
    Serial1.begin(250000, SERIAL_8N2);
    Serial1.write(0);
  }
  x = s;
  while (x < s + (512 / transSteps)) {

    Serial1.write(smV[x]);
    x++;
  }
}
void encoder() {
  if (turn) {
    deactivateFaders();
    if (turnRnL) {
      if (mode == 3) {
        midiButtonSend(true, true, 8);
      }
      if (DevChn) {
        dev = calc(dev, 1, noD);
        Ch = deviceStart[dev];
      } else {
        Ch = calc(Ch, 1, ChN + xCh);
      }
    } else {
      if (mode == 3) {
        midiButtonSend(true, true, 9);
      }
      if (DevChn) {
        dev = calc(dev, -1, noD);
        Ch = deviceStart[dev];
      } else {
        Ch = calc(Ch, -1, ChN + xCh);
      }
    }
    turn = false;
    if (mode != 3) {
      lcd.clear();
      lcd.print(String(Ch + 1));
      lcd.setCursor(0, 1);
      for (p = 0; p < 8; p++) {
        printChannelName(p);
      }
    }
  }
}

void valueRead() {
  if (pushs[s]) {
    active[s] = ! active[s];
  }
  fader[s] = analogRead(As[s]);
  if (!active[s]) {
    if (fadeOld[s] < fader[s] - diff || fadeOld[s] > fader[s] + diff) {
      active[s] = true;
    }
  }
  if (active[s]) {
    fadeOld[s] = fader[s];
    if (false) { //pbf[GlA]) {
      valuebuffer = conv(analogRead(As[s]));
      if (targetChannel(s) >= G1S && targetChannel(s) < G1E) {

        for (g = targetChannel(s); g < G1E; g = g + ChD1) {
          values[g] = valuebuffer;
        }
        for (g = targetChannel(s); g >= G1S; g = g - ChD1) {
          values[g] = valuebuffer;
        }
      }
      values[targetChannel(s)] = valuebuffer;
      if (targetChannel(s) >= G2S && targetChannel(s) < G2E) {

        for (g = targetChannel(s); g < G2E; g = g + ChD2) {
          values[g] = valuebuffer;
        }
        for (g = targetChannel(s); g >= G2S; g = g - ChD2) {
          values[g] = valuebuffer;
        }
      }
    } else {
      values[targetChannel(s)] = conv(analogRead(As[s]));
    }
  }
}

void valueReadChange() {
  fader[s] = analogRead(As[s]);
  if (fadeOld[s] < fader[s] - diff || fadeOld[s] > fader[s] + diff) {


    fadeOld[s] = fader[s];

    midiSend(fader[s], s);
  }
}

void displayAnalog() {
  dr = values[ChN + 0];
  dg = values[ChN + 1];
  db = values[ChN + 2];
  dd = values[ChN + 3];
  dc = values[ChN + 4];
  analogWrite(R, (255 - (dr * dd / 255)));
  analogWrite(G, (255 - (dg * dd / 255)));
  analogWrite(B, (255 - (db * dd / 255)));
  analogWrite(C, dc);
}
void displayAnalog2() {
  dr = smV[ChN + 0];
  dg = smV[ChN + 1];
  db = smV[ChN + 2];
  dd = smV[ChN + 3];
  dc = smV[ChN + 4];
  analogWrite(R, (255 - (dr * dd / 255)));
  analogWrite(G, (255 - (dg * dd / 255)));
  analogWrite(B, (255 - (db * dd / 255)));
  analogWrite(C, dc);
}


byte conv(int a) {
  return (constrain(map(a, Min, Max, 0, 255), 0, 255));
}
void rotLeft() {
  if (lower && digitalRead(l) == HIGH && digitalRead(r) == LOW) {
    turn = true;
    turnRnL = true;
  }
  if (lower && digitalRead(r) == HIGH && digitalRead(l) == LOW) {
    turn = true;
    turnRnL = false;
  }
  lower = digitalRead(r) == LOW && digitalRead(l) == LOW;
}
void rotRight() {
}

void printChannelName(int s) {
  lcd.setCursor(s * 2, 1);
  lcd.print(channelNames[2 * targetChannel(s)]);
  lcd.setCursor(s * 2 + 1, 1);
  lcd.print(channelNames[2 * targetChannel(s) + 1]);
  if (!DevChn) {
    lcd.setCursor(s * 2, 0);
    lcd.print(names[2 * targetChannel(s)]);
    lcd.setCursor(s * 2 + 1, 0);
    lcd.print(names[2 * targetChannel(s) + 1]);
  }
  if (DevChn) {
    lcd.setCursor(4, 0);
    lcd.print(deviceNames[2 * dev]);
    lcd.print(deviceNames[2 * dev + 1]);
    lcd.print( " " );
    lcd.print(typeNames[deviceType[dev] * 3]);
    lcd.print(typeNames[deviceType[dev] * 3 + 1]);
    lcd.print(typeNames[deviceType[dev] * 3 + 2]);

    lcd.print( " " );
    lcd.print(String(dev));
  }
}
int targetChannel(int p) {
  if (fix[p]) {
    return ofs[p];
  }
  return calc(Ch, ofs[p], ChN + xCh);
}
void setupFlows() {
  for (c = 0; c < ChD1 + xCh; c++) {
    flows[c] = false;
    up[c] = true;
  }
  for (c = 0; c < 8; c++) {
    flows[ChD1 * c + 0] = true;
    flows[ChD1 * c + 1] = true;
    flows[ChD1 * c + 2] = true;
  }
  flows[ChN] = true;
  flows[ChN + 1] = true;
  flows[ChN + 2] = true;
}
void setupFaderNames() {
  k = 0;
  String sNames0 = "DAMADCMCDRMRDBFB";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames0.charAt(i);
  }
  k = 1;
  String sNames1 = "MasterDimmer ADJ";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames1.charAt(i);
  }
  k = 2;
  String sNames2 = "Colour-Makro ADJ";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames2.charAt(i);
  }
  k = 3;
  String sNames3 = " Dimmer   Cameo ";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames3.charAt(i);
  }
  k = 4;
  String sNames4 = "  Makro  Cameo  ";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames4.charAt(i);
  }
  k = 5;
  String sNames5 = "Dimmer Renkforce";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames5.charAt(i);
  }
  k = 6;
  String sNames6 = "Makro  Renkforce";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames6.charAt(i);
  }
  k = 7;
  String sNames7 = "MasterDimmer BAR";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames7.charAt(i);
  }
  k = 8;
  String sNames8 = "  Colours  BAR  ";
  for (i = 0; i < 16; i++) {
    faderNames[i + k * 16] = sNames8.charAt(i);
  }
}

void flow() {
  if (pbf[FlA]) {
    z++;
    if (z > spd) {
      z = 0;
      flow1();
      flow2();
    }
  }
}
void flow1() {
  for (c = Fl1S; c < Fl1E; c++) {
    if (flows[c]) {
      if (values[c] <= 0) {
        values[c] = 0;
        up[c] = true;
      }
      if (values[c] >= 255) {
        values[c] = 255;
        up[c] = false;
      }
      valuebuffer = values[c];
      if (up[c]) {
        values[c] = constrain(valuebuffer + 1, 0, 255);
      } else {
        values[c] = constrain(valuebuffer - 1, 0, 255);
      }
    }
  }
}
void flow2() {
  for (c = Fl2S; c < Fl2E; c++) {
    if (flows[c]) {
      if (values[c] <= 0) {
        values[c] = 0;
        up[c] = true;
      }
      if (values[c] >= 255) {
        values[c] = 255;
        up[c] = false;
      }
      valuebuffer = values[c];
      if (up[c]) {
        values[c] = constrain(valuebuffer + 1, 0, 255);
      } else {
        values[c] = constrain(valuebuffer - 1, 0, 255);
      }
    }
  }
}

void power() {
  if (pp) {
    MenuBackRight();
    pp = false;
    powerDown();
  }
}

void powerDown() {
  lcd.clear();
  lcd.print(F("  Now Saving... "));
  delay(1000);
  int g;
  int h;
  for (h = 0; h < 255; h++) {
    switch (h % 3) {
      case 0: lcd.print(F("Shutting Down.. ")); break;
      case 1: lcd.print(F("Shutting Down ..")); break;
      case 2: lcd.print(F("Shutting Down. .")); break;
    }
    for (g = 0; g < ChN; g++) {
      valuebuffer = values[g];
      if (valuebuffer > 0) {
        values[g] = valuebuffer - 1;
      }
    }
  }
}

void powerUp() {

}

//*********************************************




//*********************************************



//*****************************************

void simpleLoop() {

  simpleFaders();
  delay(dTime);
  transmitter2();
  buttonRead(s);
  if (pushs[s] == true) {
    if (active[s]) {
      simpleInit();
    } else {
      deactivateFaders();
      active[s] = true;
      lcd.setCursor(0, 1);
      for (i = 0; i < 16; i++) {
        lcd.print(faderNames[(s + 1) * 16 + i]);
      }
    }
  }
  displayAnalog2();

}

void simpleFaders() {
  for (k = s * noD / 8; k < (s + 1)*noD / 8; k++) {
    for (i = 0; i < typeLength[deviceType[k]]; i++) {
      c = smF[maxCh * deviceType[k] + i];
      d = smW[maxCh * deviceType[k] + i];
      if (c == 0) {
        valuebuffer = d;
      } else {
        if (c < 9) {
          valuebuffer = conv(analogRead(As[c - 1]));
        } else {
          valuebuffer = 0;
        }
      }
      smV[deviceStart[k] + i] = valuebuffer;
    }
  }
}

void simpleInit() {
  lcd.clear();
  lcd.print("Einfacher Modus" + String(page));
  lcd.setCursor(0, 1);
  deactivateFaders();
  for (i = 0; i < 16; i++) {
    lcd.print(faderNames[i]);
  }
}

