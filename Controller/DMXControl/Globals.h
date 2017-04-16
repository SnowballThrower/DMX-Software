#ifndef globals
#define globals

const int ChN = 512; // <= 512
const int xCh = 5;
const int maxCh = 12; //12 damit passend für Bar!
const int noD = 33;
const int noT = 20;




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

#endif
