#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>


//FrontPanel
int s0 = 2;
int s1 = 3;
int s2 = 4;

int sLED = 7;
int sB = 0;
int fB = 6;

int r = 5; //?
int l = 1; //?

int GbP = 0;
int FlP = 1;
int loa = 0;
int sav = 1;
int GlA = 2;
int FlA = 3;
int Enc = 4;
int Set = 5;
int Chn = 6;
int Men = 7;

//Backplate
int powerButton = 22; //?
int rx0 = 8; //MIDI In
int tx0 = 9; //unused
int rx1 = 10; //unused
int tx1 = 11; //DMX Out


//Display
const int R = 12;
const int G = 13;
const int B = 14;

const int C = 15;
LiquidCrystal lcd(16, 17, 18, 19, 20, 21);

int pc6 = 22; //unused
int pc7 = 23; //unused


//Fader
int As[] = {A0, A1, A2, A3, A4, A5, A6, A7};

//*************************************************

const int ChN = 512; // <= 512
const int xCh = 5;

byte values[ChN + xCh];
bool flows[ChN + xCh];

char names[(ChN + xCh) * 2];


const int maxCh = 12; //12 damit passend für Bar!
const int noD = 33;
const int noT = 20;
char deviceNames[noD * 2];
byte typeLength[noD];
int deviceStart[noD];
byte deviceType[noD];
char channelNames[(ChN + xCh) * 2];
char typeNames[noT * 3];
char typeChnNames[maxCh  * 2];
byte smF[maxCh * noT];
byte smW[maxCh * noT];

int FLASHED = 2000;
int flashNumber = 176;
int CHSTART = 0;
int TYPSTART = 1000;
int NAMESTART = 1100;
int DEVSTART = 1800;
int tpB = 4;
int tpD = 5;
int tpN = 4;


int barStart = 100 - 1;
int barEnd = 196 - 1;
int barLength;
int lowNotes[] = {33, 45, 57, 21};
int targetCh;
int segments = 24;

byte receivemidi[3];
byte midiCh;
byte midiCC;
byte midiVal;
byte noteCC;
const byte ControlChange = 176;
const byte Note_On = 0x90;
const byte Note_Off = 0x80;
const byte ProgramChange = 0xC0;
const byte PITCH_BEND = 0xE0;
bool recognize;

int fadeOld[8];
int fader[8];
bool fix[8];
int ofs[8];
bool active[8];

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

bool pushp;
bool pbp;
bool pp;
bool pushs[8];
bool pushf[8];
bool pbs[8];
bool pf[8];
bool werte[8];
bool pbf[8];

//*********************
//constants
int diff = 6;
int Min = 10;
int Max = 1018;
double dTime = 0; //0.1
int transSteps = 32;
//*********************
//counters
int t = 0; //transmit
int s = 0;
int startProz = 0;
int c = 0; //u.a. simpleFader
int d = 0; //simpleFader
int p; //displayNames
int midicount = 0;
int x; //transmit
int z = 0; //flow
int i = 0;
int k = 0;
int e; //calc
int twoBytes;
byte lByte;
byte hByte;
int schritt = 0;
int wdh = 0;
double sendingTime = 0;
byte inByte; //Midi in
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
bool midiActive = false;
//*********************
// simpleMode
byte smV[ChN + xCh];
const int noP = 1;
int page = 0;
char faderNames[9 * 16 * noP];

//*************************************************

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
  digitalWrite(sLED, active[s]);
  switch (mode) {
    case 0: simpleLoop(); break;
    case 1:
    case 2: channelLoop(); break;
    case 3: remoteLoop(); break;
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

  

  delay(dTime);
  transmitter();
  buttonRead(s);

  midiActive = true;

  valueReadChange();

  displayAnalog();

}

//****************************************************

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    inByte = Serial.read();
    if (inByte > 127) {
      midicount = 0;
      if (inByte >= ControlChange) {
        if (inByte < 192) {
          noteCC = 0;
          receivemidi[0] = inByte - ControlChange;
        }
      }
      if (inByte >= Note_On) {
        if (inByte < 0xA0) {
          noteCC = 1;
          receivemidi[0] = inByte - 0x90;
        }
      }
      if (inByte >= Note_Off) {
        if (inByte < 0x90) {
          noteCC = 2;
          receivemidi[0] = inByte - 0x80;
        }
      }
      if (inByte >= PITCH_BEND) {
        if (inByte < 0xF0) {
          noteCC = 3;
          receivemidi[0] = inByte - ProgramChange;
        }
      }
    }
    if (inByte < 128) {
      midicount++;
      if (midicount < 3) {
        receivemidi[midicount] = inByte;
      }
      if (midicount == 2) {
        if (midiActive) {
          midiCh = receivemidi[0];
          if (noteCC == 0) {
            targetCh = receivemidi[1] + 128 * midiCh;
            values[targetCh] = 2 * receivemidi[2];
          } else {
            if (noteCC == 3) {
              handleProgramChange(receivemidi[1],receivemidi[2]);
            } else {
              if (receivemidi[0] < 3) {
                if (receivemidi[1] >= lowNotes[midiCh]) {
                  barLength = (barEnd - barStart) / 3;
                  if (receivemidi[1] < lowNotes[midiCh] + barLength) {
                    targetCh = barStart + 3 * receivemidi[1] + midiCh - 3 * lowNotes[midiCh];

                    if (noteCC == 2) {
                      values[targetCh] = 0;
                    }
                    if (noteCC == 1) {
                      values[targetCh] = 2 * receivemidi[2];
                    }
                  }
                }
              }
              if (midiCh == 3) {
                barLength = (barEnd - barStart);
                if (receivemidi[1] >= lowNotes[3]) {
                  if (receivemidi[1] < lowNotes[3] + barLength) {
                    targetCh = barStart + 3 * receivemidi[1] + receivemidi[1] / segments - lowNotes[3];

                    if (noteCC == 2) {
                      values[targetCh] = 0;
                    }
                    if (noteCC == 1) {
                      values[targetCh] = 2 * receivemidi[2];
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void midiSend(int val, byte fad) {
  Serial.write(noteCC + fad);
  Serial.write(val / 32);
  Serial.write(val % 32);
}

void midiButtonSend(bool fs, bool hi, byte num) {
  if (hi) {
    Serial.write(Note_On);
    Serial.write(8 * fs + num);
  } else {
    Serial.write(Note_Off);
    Serial.write(8 * fs + num);
  }
}

void handleProgramChange(byte r1, byte r2) {
  if (true){//r1 < 64) {
    bool col = r1 < 32;
    byte line = (r1 % 32) / 2;
    byte val = r2 * 2 + r1 % 2;
    lcd.setCursor(line, col);
    lcd.print((char)val);
  }
  else{
    
  }
}

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
  if (pbs[s] && !pushs[s]) {
    pushs[s] = true;
  } else {
    pushs[s] = pbs[s];
  }
  if (pbf[s] && !pushf[s]) {
    pushf[s] = true;
    pf[s] = true;
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
      if (DevChn) {
        dev = calc(dev, 1, noD);
        Ch = deviceStart[dev];
      } else {
        Ch = calc(Ch, 1, ChN + xCh);
      }
    } else {
      if (DevChn) {
        dev = calc(dev, -1, noD);
        Ch = deviceStart[dev];
      } else {
        Ch = calc(Ch, -1, ChN + xCh);
      }
    }
    turn = false;
    lcd.clear();
    lcd.print(String(Ch + 1));
    lcd.setCursor(0, 1);
    for (p = 0; p < 8; p++) {
      printChannelName(p);
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

void selector(bool a, bool b, bool c) {
  digitalWrite(s0, a);
  digitalWrite(s1, b);
  digitalWrite(s2, c);
}

void select(int multi) {
  switch (multi) {
    case 0: selector(0, 0, 0); break;
    case 1: selector(1, 0, 0); break;
    case 2: selector(0, 1, 0); break;
    case 3: selector(1, 1, 0); break;
    case 4: selector(0, 0, 1); break;
    case 5: selector(1, 0, 1); break;
    case 6: selector(0, 1, 1); break;
    case 7: selector(1, 1, 1); break;
  }
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
int calcalt(int s, int zahl, int Max) {
  return (zahl + Max + s) % Max;
}
int calc(int w, int zahl, int Max) {
  e = zahl + w;
  while (e >= Max) {
    e = e - Max;
  }
  while (e < 0) {
    e = e + Max;
  }
  return e;
}
void deactivateFaders() {

  for (i = 0; i < 8; i++) {
    active[i] = false;
  }
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


  startProz++;//1
  select(startProz);

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


  startProz++; //2
  select(startProz);

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

  startProz++;//4
  select(startProz);

  for (i = 0; i < noD; i++) {
    saveDevice(i);
  }
  startProz++;//5
  select(startProz);
  for (i = 0; i < noT; i++) {
    saveType(i);
  }

  startProz++;//6
  select(startProz);
  EEPROM.write(FLASHED, flashNumber);
}

void setupTypes() {
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
  typeChnNames[3] = 'M';
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
  startProz++;//3
  select(startProz);
}
void setupChannels() {

  startProz++;
  select(startProz);
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

  startProz++;
  select(startProz);

  loadDevTyp();

  startProz++;
  select(startProz);
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

void printMenu() {
  lcd.setCursor(0, 0);
  lcd.print(MenuName(MenuPosition));
}

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

void MenuBackRight() { // to next menu-region
  MenuBack();
  MenuRight();
  MenuClick();
  in = true;
}

void MenuBack() { // jump back
  MenuPosition = BackJump[MenuPosition];
}

void MenuStart() { // to first menu-region
  MenuPosition = 0;
  MenuRight();
  MenuClick();
  in = true;
}

int sub(int zahl, int Min, int Max) {
  if (zahl - 1 < Min) {
    return Max - Min + zahl;
  }
  return zahl - 1;
}

int add(int zahl, int Min, int Max) {
  if (zahl + 1 > Max) {
    return Min - Max + zahl;
  }
  return zahl + 1;
}

//*********************************************

void loadValues() {
  deactivateFaders();
  for (i = 0; i < ChN; i++) {
    values[i] = EEPROM.read(CHSTART + i);
  }
}

void saveValues() {
  for (i = 0; i < ChN; i++) {
    if (values[i] != EEPROM.read(CHSTART + i)) {
      EEPROM.write(CHSTART + i, values[i]);
    }
  }
}

char getTypeName(int type, int ziffer) {
  return EEPROM.read(TYPSTART + tpB * type + ziffer + 1);
}
int getTypeLength(int type) {
  return EEPROM.read(TYPSTART + tpB * type);
}
void readTypeChannelNames(int type) {
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
  for (i = 0; i < typeLength[type]; i++) {
    smF[type * maxCh + i] =                       EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 2);
    smW[type * maxCh + i] =                       EEPROM.read(NAMESTART + tpN * type * maxCh + tpN * i + 3);
  }

}
char getChannelName(int type, int channel, int ziffer) {
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

