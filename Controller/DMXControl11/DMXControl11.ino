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


const int maxCh = 12;
const int noD = 33;
const int noT = 20;
char deviceNames[noD * 2];
byte typeLength[noD];
int deviceStart[noD];
byte deviceType[noD];
char channelNames[(ChN + xCh) * 2];
char typeNames[noT * 3];
char typeChnNames[maxCh * noT * 2];
byte smF[maxCh * noT];
byte smW[maxCh * noT];

int FLASHED = 2000;
int flashNumber = 169;
int CHSTART = 0;
int TYPSTART = 1000;
int NAMESTART = 1100;
int DEVSTART = 1800;
int tpB = 4;
int tpD = 5;
int tpN = 4;


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

byte receivemidi[3];

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
  values[ChN+0]=255;
  values[ChN+1]=255;
  values[ChN+2]=0;
  values[ChN+3]=255;
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
  s=0;
  printMenu();
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
    case 3:
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
    case 3:
    default: break;
  }
}
void channelLoop() {

  

  encoder();

  menu();

  delay(dTime);
  transmitter();
  buttonRead(s);

  midiActive = pbf[GlA];

  flow();

  valueRead();

  displayAnalog();

}

//****************************************************

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    inByte = Serial.read();
    if (inByte > 127) {
      if (inByte >= 176 && inByte < 192) {
        midicount = 0;
        receivemidi[0] = inByte - 176;
      }
    }
    if (inByte < 128) {
      midicount++;
      if (midicount < 3) {
        receivemidi[midicount] = inByte;
      }
      if (midicount == 2) {
        if (midiActive) {
          values[(receivemidi[1] + 128 * receivemidi[0])] = 2 * receivemidi[2];
        }
      }
    }
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
      Ch = calc(Ch, 1, ChN + xCh);
    } else {
      Ch = calc(Ch, -1, ChN + xCh);
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
  lcd.setCursor(s * 2, 0);
  lcd.print(names[2 * targetChannel(s)]);
  lcd.setCursor(s * 2 + 1, 0);
  lcd.print(names[2 * targetChannel(s) + 1]);
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
  deviceNames[6] = 'R';
  deviceNames[7] = '1';
  deviceNames[8] = 'R';
  deviceNames[9] = '2';
  deviceNames[10] = 'B';
  deviceNames[11] = '1';
  deviceNames[12] = 'B';
  deviceNames[13] = '2';
  deviceNames[noD * 2 - 2] = 'D';
  deviceNames[noD * 2 - 1] = 'i';

  deviceType[0] = 1;  //ADJ
  deviceType[1] = 3;  //Cameo
  deviceType[2] = 8;  //Outdoor
  deviceType[3] = 2;  //Renkforce
  deviceType[4] = 2;  //Renkforce
  deviceType[5] = 4;  //Bar
  deviceType[6] = 4;  //Bar
  deviceType[noD - 1] = noT - 1;

  deviceStart[0] = 1-1;   //ADJ
  deviceStart[1] = 10-1;  //Cameo
  deviceStart[2] = 20-1;  //Outdoor
  deviceStart[3] = 30-1;  //Renkforce
  deviceStart[4] = 40-1;  //Renkforce
  deviceStart[5] = 50-1;  //Bar
  deviceStart[6] = 75-1;  //Bar
  
  deviceStart[noD - 1] = ChN;


  for (i = 0; i < (ChN + xCh) * 2; i++) {
    names[i] = '|';
    channelNames[i] = '-';
  }


  startProz++; //2
  select(startProz);
  
  setupTypes();


  for (k = 0; k < noD; k++) {
    for (i = deviceStart[k]; i < (deviceStart[k] + typeLength[deviceType[k]]); i++) {
      names[i * 2] = deviceNames[k * 2];
      names[i * 2 + 1] = deviceNames[k * 2 + 1];
      channelNames[i * 2] = typeChnNames[(i - deviceStart[k]) * 2 + 2 * maxCh * deviceType[k]];
      channelNames[i * 2 + 1] = typeChnNames[(i - deviceStart[k]) * 2 + 2 * maxCh * deviceType[k] + 1];
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
  int tp = 1;   //ADJ
  typeLength[tp] = 7;
  typeNames[tp * 3 + 0] = 'A';
  typeNames[tp * 3 + 1] = 'D';
  typeNames[tp * 3 + 2] = 'J';
  typeChnNames[tp * 2 * maxCh + 0] = 'R';
  typeChnNames[tp * 2 * maxCh + 1] = 'o';
  typeChnNames[tp * 2 * maxCh + 2] = 'G';
  typeChnNames[tp * 2 * maxCh + 3] = 'r';
  typeChnNames[tp * 2 * maxCh + 4] = 'B';
  typeChnNames[tp * 2 * maxCh + 5] = 'l';
  typeChnNames[tp * 2 * maxCh + 6] = 'M';
  typeChnNames[tp * 2 * maxCh + 7] = 'a';
  typeChnNames[tp * 2 * maxCh + 8] = 'S';
  typeChnNames[tp * 2 * maxCh + 9] = 'p';
  typeChnNames[tp * 2 * maxCh + 10] = 'M';
  typeChnNames[tp * 2 * maxCh + 11] = 'o';
  typeChnNames[tp * 2 * maxCh + 12] = 'D';
  typeChnNames[tp * 2 * maxCh + 13] = 'm';
  smF[tp * maxCh + 3] = 2;
  smF[tp * maxCh + 6] = 1;

  tp = 2;   //Renkforce
  typeLength[tp] = 10;
  typeNames[tp * 3 + 0] = 'R';
  typeNames[tp * 3 + 1] = 'f';
  typeNames[tp * 3 + 2] = 'P';
  typeChnNames[tp * 2 * maxCh + 0] = 'D';
  typeChnNames[tp * 2 * maxCh + 1] = 'm';
  typeChnNames[tp * 2 * maxCh + 2] = 'R';
  typeChnNames[tp * 2 * maxCh + 3] = 'o';
  typeChnNames[tp * 2 * maxCh + 4] = 'G';
  typeChnNames[tp * 2 * maxCh + 5] = 'r';
  typeChnNames[tp * 2 * maxCh + 6] = 'B';
  typeChnNames[tp * 2 * maxCh + 7] = 'l';
  typeChnNames[tp * 2 * maxCh + 8] = 'W';
  typeChnNames[tp * 2 * maxCh + 9] = 'e';
  typeChnNames[tp * 2 * maxCh + 10] = 'A';
  typeChnNames[tp * 2 * maxCh + 11] = 'm';
  typeChnNames[tp * 2 * maxCh + 12] = 'S';
  typeChnNames[tp * 2 * maxCh + 13] = 'r';
  typeChnNames[tp * 2 * maxCh + 14] = 'M';
  typeChnNames[tp * 2 * maxCh + 15] = 'a';
  typeChnNames[tp * 2 * maxCh + 16] = 'M';
  typeChnNames[tp * 2 * maxCh + 17] = 's';
  typeChnNames[tp * 2 * maxCh + 18] = 'D';
  typeChnNames[tp * 2 * maxCh + 19] = 'c';
  smF[tp * maxCh + 0] = 3;
  smF[tp * maxCh + 7] = 4;

  tp = 3;   //Cameo
  typeLength[tp] = 9;
  typeNames[tp * 3 + 0] = 'C';
  typeNames[tp * 3 + 1] = 'a';
  typeNames[tp * 3 + 2] = 'P';
  typeChnNames[tp * 2 * maxCh] = 'D';
  typeChnNames[tp * 2 * maxCh + 1] = 'm';
  typeChnNames[tp * 2 * maxCh + 2] = 'S';
  typeChnNames[tp * 2 * maxCh + 3] = 'r';
  typeChnNames[tp * 2 * maxCh + 4] = 'R';
  typeChnNames[tp * 2 * maxCh + 5] = 'o';
  typeChnNames[tp * 2 * maxCh + 6] = 'G';
  typeChnNames[tp * 2 * maxCh + 7] = 'r';
  typeChnNames[tp * 2 * maxCh + 8] = 'B';
  typeChnNames[tp * 2 * maxCh + 9] = 'l';
  typeChnNames[tp * 2 * maxCh + 10] = 'W';
  typeChnNames[tp * 2 * maxCh + 11] = 'e';
  typeChnNames[tp * 2 * maxCh + 12] = 'A';
  typeChnNames[tp * 2 * maxCh + 13] = 'm';
  typeChnNames[tp * 2 * maxCh + 14] = 'U';
  typeChnNames[tp * 2 * maxCh + 15] = 'V';
  typeChnNames[tp * 2 * maxCh + 16] = 'M';
  typeChnNames[tp * 2 * maxCh + 17] = 'a';
  smF[tp * maxCh + 0] = 5;
  smF[tp * maxCh + 8] = 6;

  tp = 4;   //Bar24
  typeLength[tp] = 24;
  typeNames[tp * 3 + 0] = 'B';
  typeNames[tp * 3 + 1] = 'a';
  typeNames[tp * 3 + 2] = 'r';
  for (i = 0; i < 8; i++) {
    typeChnNames[tp * 2 * maxCh + i * 6 + 0] = 'R';
    typeChnNames[tp * 2 * maxCh + i * 6 + 1] = (char) i + 1 + 48;
    typeChnNames[tp * 2 * maxCh + i * 6 + 2] = 'G';
    typeChnNames[tp * 2 * maxCh + i * 6 + 3] = (char) i + 1 + 48;
    typeChnNames[tp * 2 * maxCh + i * 6 + 4] = 'B';
    typeChnNames[tp * 2 * maxCh + i * 6 + 5] = (char) i + 1 + 48;
  }

  tp = 5;   //Bar24 forts.
  typeNames[tp * 3 + 0] = '1';
  typeNames[tp * 3 + 1] = 'B';
  typeNames[tp * 3 + 2] = 'a';

  tp = 6;   //Bar24 forts.
  typeNames[tp * 3 + 0] = '2';
  typeNames[tp * 3 + 1] = 'B';
  typeNames[tp * 3 + 2] = 'a';

  tp = 7;   //Bar5
  typeLength[tp] = 5;
  typeNames[tp * 3 + 0] = 'B';
  typeNames[tp * 3 + 1] = 'a';
  typeNames[tp * 3 + 2] = '5';
  typeChnNames[tp * 2 * maxCh + 0] = 'R';
  typeChnNames[tp * 2 * maxCh + 1] = 'o';
  typeChnNames[tp * 2 * maxCh + 2] = 'G';
  typeChnNames[tp * 2 * maxCh + 3] = 'r';
  typeChnNames[tp * 2 * maxCh + 4] = 'B';
  typeChnNames[tp * 2 * maxCh + 5] = 'l';
  typeChnNames[tp * 2 * maxCh + 6] = 'D';
  typeChnNames[tp * 2 * maxCh + 7] = 'm';
  typeChnNames[tp * 2 * maxCh + 8] = 'S';
  typeChnNames[tp * 2 * maxCh + 9] = 'r';
  smF[tp * maxCh + 3] = 7;
  smF[tp * maxCh + 0] = 8;
  smF[tp * maxCh + 1] = 8;
  smF[tp * maxCh + 2] = 8;
   
  tp = 8;   //Outdoor
  typeLength[tp] = 7;
  typeNames[tp * 3 + 0] = 'O';
  typeNames[tp * 3 + 1] = 'u';
  typeNames[tp * 3 + 2] = 't';
  typeChnNames[tp * 2 * maxCh] = 'D';
  typeChnNames[tp * 2 * maxCh + 1] = 'm';
  typeChnNames[tp * 2 * maxCh + 2] = 'R';
  typeChnNames[tp * 2 * maxCh + 3] = 'M';
  typeChnNames[tp * 2 * maxCh + 4] = 'G';
  typeChnNames[tp * 2 * maxCh + 5] = 'r';
  typeChnNames[tp * 2 * maxCh + 6] = 'B';
  typeChnNames[tp * 2 * maxCh + 7] = 'l';
  typeChnNames[tp * 2 * maxCh + 8] = 'W';
  typeChnNames[tp * 2 * maxCh + 9] = 'e';
  typeChnNames[tp * 2 * maxCh + 10] = 'S';
  typeChnNames[tp * 2 * maxCh + 11] = 'r';
  typeChnNames[tp * 2 * maxCh + 12] = 'P';
  typeChnNames[tp * 2 * maxCh + 13] = 'r';
  
  tp = noT - 1;
  typeLength[tp] = xCh;
  typeNames[tp * 3 + 0] = 'D';
  typeNames[tp * 3 + 1] = 'i';
  typeNames[tp * 3 + 2] = 's';
  typeChnNames[tp * 2 * maxCh] = 'R';
  typeChnNames[tp * 2 * maxCh + 1] = 'o';
  typeChnNames[tp * 2 * maxCh + 2] = 'G';
  typeChnNames[tp * 2 * maxCh + 3] = 'r';
  typeChnNames[tp * 2 * maxCh + 4] = 'B';
  typeChnNames[tp * 2 * maxCh + 5] = 'l';
  typeChnNames[tp * 2 * maxCh + 6] = 'D';
  typeChnNames[tp * 2 * maxCh + 7] = 'm';
  typeChnNames[tp * 2 * maxCh + 8] = 'C';
  typeChnNames[tp * 2 * maxCh + 9] = 'o';
  smW[tp * maxCh + 0] = 255;
  smW[tp * maxCh + 1] = 255;
  smW[tp * maxCh + 2] = 0;
  smW[tp * maxCh + 3] = 255;
  smW[tp * maxCh + 4] = 55;

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
    for (i = deviceStart[k]; i < (deviceStart[k] + typeLength[deviceType[k]]); i++) {
      names[i * 2] = deviceNames[k * 2];
      names[i * 2 + 1] = deviceNames[k * 2 + 1];
      channelNames[i * 2] = typeChnNames[(i - deviceStart[k]) * 2 + 2 * maxCh * deviceType[k]];
      channelNames[i * 2 + 1] = typeChnNames[(i - deviceStart[k]) * 2 + 2 * maxCh * deviceType[k] + 1];
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
  for (i = 0; i < ChN + xCh; i++) {
    values[i] = EEPROM.read(CHSTART + i);
  }
}

void saveValues() {
  for (i = 0; i < ChN + xCh; i++) {
    if (values[i] != EEPROM.read(CHSTART + i)) {
      EEPROM.write(CHSTART + i, values[i]);
    }
  }
}

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
    if (typeChnNames[type * 2 * maxCh + 2 * i] !=     EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i)) {
      EEPROM.write(NAMESTART + type * tpN * maxCh + tpN * i,      typeChnNames[type * 2 * maxCh + 2 * i]);
    }
    if (typeChnNames[type * 2 * maxCh + 2 * i + 1] != EEPROM.read(NAMESTART + type * tpN * maxCh + tpN * i + 1)) {
      EEPROM.write(NAMESTART + type * tpN * maxCh + tpN * i + 1,  typeChnNames[type * 2 * maxCh + 2 * i + 1]);
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
    loadType(i);
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

