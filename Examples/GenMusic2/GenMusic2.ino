/*
* GenMusic2
* Exploring the upper range of the instrument
* moderndevice.com
*/

#include <Fluxamasynth.h>
#include <PgmChange.h>

Fluxamasynth synth;
int count =0;
int count2 = 0;
int s = 120;

void setup() {
  // Serial1.begin(31250);             // Uncomment if using a Feather M0 board
  delay(20);
  synth.midiReset();
  synth.setMasterVolume(50);
  int instrument = 2;
  randomSeed(millis()+analogRead(1));
  for (int i=0; i<4; i++) {
      synth.programChange(0, i, instrument);
      synth.setReverb(i, 1, 127, 25);
      synth.pan(i, 127/(i+1));
  }
}

void loop() {
  int d = random(75)+75;
  for (int i=0; i<5; i++) {
    synth.programChange(0, i, random(127)); //random(127)
  }
  for (int i=1; i<7; i++) {
    section(s, 100);
  }
  s = random(10)+117;
  
}

void section(int n, int d1) {
for (int i=n; i<n+8; i++) {
    count++;
    count2+=3;
    synth.noteOn(0, i, 127);
    if ((count2%2) ==0) synth.noteOn(1, i-3, 127);
    if ((count%2) ==0) {
       synth.noteOn(2, i-2, 127);
    }
    if ((count%3) ==0) {
       synth.noteOn(3, i-5, 127);
    }
   if ((count2%6) ==0)synth.noteOn(4, i+2, 127);
    delay(100);
    synth.noteOff(0, i);
    synth.noteOff(1, i-3);
    synth.noteOff(2, i-2);
    synth.noteOff(3, i-5);
    synth.noteOff(4, i+2);
  }
}
