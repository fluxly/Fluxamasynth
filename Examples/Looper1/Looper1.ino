// Fluxamasynth out of sync tape loop example
// Inspired by Eno's Music for Airports #2: F2 G#2 C3 C#3 D#3 F3 G#3
// Won't make sense without 10k poteniometers on pins A0-A5
// https://vimeo.com/252388252

#include "Fluxamasynth.h"
Fluxamasynth synth;

int tones[6] = { 29, 32, 36, 37, 39, 41 };
int instruments[6] = { 55, 55, 55, 55, 55, 55 };
int tapeLength[6] = { 30, 31, 32, 33, 34, 35  };
int startingPoint[6] = { 0, 1, 2, 3, 4, 5 };
int length[6] = { 100, 110, 120, 130, 140, 150 };
int lengthCount[6] = { 0, 0, 0, 0, 0, 0 };
int tempo[6] = { 1000, 2000, 3000, 4000, 5000, 6000 };
int tempoCount[6] = { 0, 0, 0, 0, 0, 0 };
long int startTime = 0;

void setup() {
  // Serial1.begin(31250);             // Uncomment if using a Feather M0 board
  Serial.begin(9600);
  int seed = 0;
  for (int i=0; i<6; i++) {
    seed += analogRead(i);
  }

  randomSeed(seed);
  delay(100);
  for (int i=0; i<6; i++) {
    synth.setChannelVolume(i, 255 );  
    synth.allNotesOff(i);    
    synth.setReverb(i, 5, 64, 0);
    synth.pan(5-i,127/(6*(i+1)));
  }
  changeProgram();
  startTime = 0;
}

void changeProgram() {
   for (int i=0; i<6; i++) {
      synth.programChange(0, i,random(127));
   }
}

void loop() {
  if (millis() > (startTime + 20000)) {
    changeProgram();
    startTime = millis();
    Serial.println("changeProgram");
  }
  int ledTempo = 0;
  for (int i=0; i<6; i++) {
    int val = analogRead(i);
    tempo[i] = 5100 - map(val, 0, 1023, 100, 5000); 
    tempoCount[i]++;
    if (tempoCount[i] > tempo[i]) {
      if (lengthCount[i] == 0) {
        synth.noteOn(i, tones[i], 127);
      }
      lengthCount[i]++;
      if (lengthCount[i] > length[i]) {
        synth.noteOff(i, tones[i]);
        lengthCount[i] = 0;
        tempoCount[i] = 0;
      }
    }
  }  
  delay(1);
}
