#ifndef statusse
#define statusse

byte values[ChN + xCh];
byte smV[ChN + xCh]; //simpleModeValues


bool pushp;
bool pushButtonP;
bool pp;
bool pushs[8];
bool pushButtonS[8];
bool pushf[8];
bool pushButtonF[8];
bool pf[8];
bool werte[8];


int fadeOld[8];
int fader[8];
bool fix[8];
int ofs[8];
bool active[8];
byte led[8];

bool midiActive = false;

int t = 0; //transmit

int Ch = 0;
int dev = 0;
bool DevChn = true;

#endif
