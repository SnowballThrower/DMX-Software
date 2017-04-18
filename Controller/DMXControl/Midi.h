#ifndef mymidi
#define mymidi

#include <Arduino.h>


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

int midicount = 0;
byte inByte; //Midi in


void handlePitch(byte r1, byte r2) {
  bool line = r1 < 32;
  byte col = (r1 % 32) / 2;
  byte val = r2 * 2 + r1 % 2;
  if (r1 < 64) {
    lcd.setCursor(col, line);
    lcd.print((char)val);
  }
  else {
    if (col < 8) {
      led[col] = val;
    }
  }
}

void firstByte() {
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
      receivemidi[0] = inByte - PITCH_BEND;
    }
  }
}

void bar() {
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

void handleMidiData() {
  midiCh = receivemidi[0];
  if (noteCC == 0) {
    targetCh = receivemidi[1] + 128 * midiCh;
    values[targetCh] = 2 * receivemidi[2];
  } else {
    if (noteCC == 3) {
      handlePitch(receivemidi[1], receivemidi[2]);
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
        bar();
      }
    }
  }
}

void nextBytes() {
  midicount++;
  if (midicount < 3) {
    receivemidi[midicount] = inByte;
  }
  if (midicount == 2) {
    if (midiActive) {
      handleMidiData();
    }
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    inByte = Serial.read();
    if (inByte > 127) {
      firstByte();
    }
    if (inByte < 128) {
      nextBytes();
    }
  }
}

void midiSend(int val, byte fad) {
  Serial.write(ControlChange + fad);
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
  Serial.write(0);
}

#endif
