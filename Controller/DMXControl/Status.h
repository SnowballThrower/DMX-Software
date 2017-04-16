#ifndef statusse
#define statusse

byte values[ChN + xCh];
bool flows[ChN + xCh];


bool pushp;
bool pbp;
bool pp;
bool pushs[8];
bool pushf[8];
bool pbs[8];
bool pf[8];
bool werte[8];
bool pbf[8];


int fadeOld[8];
int fader[8];
bool fix[8];
int ofs[8];
bool active[8];
byte led[8];

bool midiActive = false;

#endif
