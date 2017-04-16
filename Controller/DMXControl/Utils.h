#ifndef utils
#define utils

#include "Status.h"

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
void deactivateFaders() {
  int i;
  for (i = 0; i < 8; i++) {
    active[i] = false;
  }
}

#endif
