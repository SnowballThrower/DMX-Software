#ifndef utils
#define utils

#include "Status.h"

//AnalogRead
int diff = 6;
int Min = 10;
int Max = 1018;


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
int calcalt(int s, int zahl, int Max) {
  return (zahl + Max + s) % Max;
}
int calc(int w, int zahl, int Max) {
  int e = zahl + w;
  while (e >= Max) {
    e = e - Max;
  }
  while (e < 0) {
    e = e + Max;
  }
  return e;
}

int targetChannel(int p) {
  if (fix[p]) {
    return ofs[p];
  }
  return calc(Ch, ofs[p], ChN + xCh);
}

void deactivateFaders() {
  int i;
  for (i = 0; i < 8; i++) {
    active[i] = false;
  }
}

byte conv(int a) {
  return (constrain(map(a, Min, Max, 0, 255), 0, 255));
}

void valueRead(int s) {
  byte valuebuffer;
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
/*
    if (false) { //pushButtonF[GlA]) {
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
    } else {*/
      values[targetChannel(s)] = conv(analogRead(As[s]));
//    }
  }
}

void valueReadChange(int s) {
  fader[s] = analogRead(As[s]);
  if (fadeOld[s] < fader[s] - diff || fadeOld[s] > fader[s] + diff) {


    fadeOld[s] = fader[s];

//    midiSend(fader[s], s);
  }
}


#endif
