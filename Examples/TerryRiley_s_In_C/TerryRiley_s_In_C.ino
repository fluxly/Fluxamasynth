// 
// Terry Riley's In C
// 
// This sketch implements Terry Riley's "In C" composition for 
// the Fluxamasynth Shield.. 
// For the score and more background visit:
// http://en.wikipedia.org/wiki/In_C
// http://imslp.org/wiki/In_C_(Riley,_Terry)
//
// http://moderndevice.com/product/fluxamasynth-shield/
// https://github.com/moderndevice/Fluxamasynth-Shield
//

#include <Fluxamasynth.h>
#include <PgmChange.h>

#define numInstruments  13
#define numParts 53

Fluxamasynth synth;

// The PgmChange.h header defines all of these instrument names
int instrument[numInstruments] = {
  BANK0_Vibraphone, 
  BANK0_Marimba,
  BANK0_ElPiano1,
  BANK0_Vibraphone,
  BANK0_ElPiano1,
  BANK0_ElPiano1,
  BANK0_Vibraphone,
  BANK0_Marimba,
  BANK0_ElGrd_Piano3,
  BANK0_Vibraphone, 
  BANK0_Marimba,
  BANK0_SynthBass1,
  BANK0_Grand_Piano1
    //12, 13, 5, 12, 5, 5, 12, 13, 3, 12, 13, 39
};

byte score[] = {
  0, 1, 255,                                             // part 0
  60, 1, 64, 3, 60, 1, 64, 3, 60, 1, 64, 3, 255,         // part 1
  60, 1, 64, 1, 65, 2, 64, 2, 0, 3, 255,                 // part 2
  0, 2, 64, 2, 65, 2, 64, 2, 255,                        // part 3
  0, 2, 64, 2, 65, 2, 67, 2, 255,                        // part 4
  64, 2, 65, 2, 67, 2, 0, 2, 255,                        // part 5
  72, 16, 72, 16, 255,                                   // part 6
  0, 14, 60, 1, 60, 1, 60, 2, 0, 18, 255,                // part 7
  67, 24, 65, 16, 65, 16, 255,                           // part 8
  71, 1, 67, 1, 0, 14, 255,                              // part 9
  71, 1, 67, 1, 255,                                     // part 10
  65, 1, 67, 1, 71, 1, 67, 1, 71, 1, 67, 1, 255,         // part 11
  65, 2, 67, 2, 71, 16, 71, 4, 255,                      // part 12
  71, 1, 67, 3, 67, 1, 65, 1, 67, 2, 0, 3, 67, 7, 255,   // part 13
  72, 16, 71, 16, 67, 16, 66, 16, 255,                   // part 14
  67, 1, 0, 15, 255,                                     // part 15
  67, 1, 71, 1, 72, 1, 71, 1, 255,                       // part 16
  71, 1, 72, 1, 71, 1, 72, 1, 71, 1, 0, 1, 255,          // part 17
  64, 1, 68, 1, 64, 1, 68, 1, 64, 3, 64, 2, 255,         // part 18
  0, 6, 79, 6, 255,                                      // part 19
  64, 1, 66, 1, 64, 1, 66, 1, 57, 3, 64, 1, 65, 1, 
  64, 1, 65, 1, 64, 1, 255,                          // part 20
  66, 12, 255,                                           // part 21
  64, 6, 64, 6, 64, 6, 64, 6, 64, 6, 66, 6, 67, 6, 
  69, 6, 71, 2, 255,                              // part 22
  64, 2, 66, 6, 66, 6, 66, 6, 66, 6, 67, 6, 
  69, 6, 71, 6, 255,                              // part 23
  64, 2, 66, 2, 67, 6, 67, 6, 67, 6, 67, 6, 
  67, 6, 69, 6, 71, 2, 255,                       // part 24
  64, 2, 66, 2, 67, 2, 69, 6, 69, 6, 69, 6, 
  69, 6, 69, 6, 71, 6, 255,                       // part 25
  64, 2, 66, 2, 67, 2, 69, 2, 71, 6, 71, 6, 71, 6, 
  71, 6, 71, 6, 255,                              // part 26
  64, 1, 66, 1, 64, 1, 66, 1, 67, 2, 64, 1, 
  67, 1, 66, 1, 64, 1, 66, 1, 64, 1, 255,         // part 27
  64, 1, 66, 1, 64, 1, 66, 1, 64, 3, 64, 1, 255,         // part 28
  64, 12, 67, 12, 72, 12, 255,                           // part 29
  72, 24, 255,                                           // part 30
  67, 1, 65, 1, 67, 1, 71, 1, 67, 1, 71, 1, 255,         // part 31
  65, 1, 67, 1, 65, 1, 67, 1, 71, 1, 65, 13, 67, 6, 255, // part 32
  67, 1, 65, 1, 0, 2, 255,                               // part 33
  67, 1, 65, 1, 255,                                     // part 34
  65, 1, 67, 1, 71, 1, 67, 1, 71, 1, 67, 1, 71, 1, 
  67, 1, 71, 1, 67, 1, 0, 14, 70, 4, 79, 12,
  81, 2, 79, 4, 83, 2, 79, 6, 79, 2, 76, 12, 
  79, 2, 78, 14, 0, 10, 76, 10, 77, 24, 255,        // part 35
  65, 1, 67, 1, 71, 1, 67, 1, 71, 1, 67, 1, 255,         // part 36
  65, 1, 67, 1, 255,                                     // part 37
  65, 1, 67, 1, 71, 1, 255,                              // part 38
  71, 1, 67, 1, 65, 1, 67, 1, 71, 1, 72, 1, 255,           // part 39
  71, 1, 65, 1, 255,                                     // part 40
  71, 1, 67, 1,                                          // part 41
  72, 16, 71, 16, 69, 16, 72, 16, 255,                   // part 42
  77, 1, 76, 1, 77, 1, 76, 1, 76, 2, 76, 2, 76, 2, 77, 
  1, 76, 1, 255,                                     // part 43
  77, 2, 76, 4, 76, 2, 72, 4, 255,                       // part 44
  74, 4, 74, 4, 67, 4, 255,                              // part 45
  67, 1, 74, 1, 76, 1, 74, 1, 0, 2, 67, 2, 67, 2, 0, 
  2, 67, 2, 67, 1, 74, 1, 76, 1, 74, 1, 255,         // part 46
  74, 1, 76, 1, 74, 2, 255,                              // part 47
  67, 24, 67, 16, 64, 16, 64, 4, 255,                    // part 48
  65, 1, 67, 1, 70, 1, 67, 1, 70, 1, 67, 1, 255,         // part 49
  65, 1, 67, 1, 255,                                     // part 50
  65, 1, 67, 1, 70, 1, 255,                              // part 51
  67, 1, 70, 1, 255,                                     // part 52
  70, 1, 67, 1, 255                                      // part 53
};

unsigned long  startTime;
int chance ;
int max = 0;
int spread = 0;
int targetVolume=127; 
int startPart = 1;
int partIndices[53];
int partBeats[53];
int index[numInstruments]; 
int part[numInstruments];
int beat[numInstruments];
int startDelay[numInstruments];
int note[numInstruments];
int volume[numInstruments];
int prevNote[numInstruments];
int noteCount[numInstruments];
boolean playNote[numInstruments];
boolean hitEnd[numInstruments];
boolean started[numInstruments];
boolean finished[numInstruments];
int numFinished = 0;
int tempo = 30;     // 30 to 150, center is 90

void setNextNote(int j) {
  if (index[j] > 0) {
    prevNote[j] = score[index[j]-1];
  } 
  else {
    prevNote[j] = 0;
  }
  note[j] = score[index[j]];
  if (j == 11) { 
    note[j] -= 24; 
  }
  noteCount[j] = score[index[j]+1];
  playNote[j] = true;
}

void setup() {
  // The tempo is determined by a potentiometer on Analog 0. 
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A2, HIGH);
  // Calculate number of beats in each part
  int beatCount = 0;
  int c = 3;
  partIndices[0] = 0;
  for (int i=1; i<53; i++) {
    partIndices[i]=c;
    while (score[c] != 255) {
      c++;
      beatCount+=score[c];
      c++;
    }
    c++;
    partBeats[i] = beatCount;
    beatCount=0;
  }
  synth.setMasterVolume(75);

  // Set up the individual instruments
  for (int i=0; i<numInstruments; i++) {
    synth.programChange(0, i, instrument[i]);
    //synth.programChange(0, i, 45);   // use this to override and test instruments
    pan(i,127/numInstruments*i);
    volume[i] = targetVolume;
    synth.setChannelVolume(i, volume[i]);
    synth.setReverb(i, 3, 127, 25);
    //synth.setChorus(i, 3, 64, 25, 25);
  }
  randomSeed(analogRead(0));
  for (int i=0; i<numInstruments; i++) {
    index[i] = 0;
    part[i] = 0;
    note[i] = 0;
    noteCount[i] = 0;
    startDelay[i] = random(1, 20) * partBeats[1];
    playNote[i] = true;
    started[i] = false;
  }
  synth.setChannelVolume(9,0);  // Turn off the percussion channel
}

void loop() {
  tempo = map(analogRead(A1), 0, 1024, 30, 150);

  startTime = millis();
  Serial.print(startTime);
  Serial.print(" ");
  for (int i=0; i< numInstruments; i++) { 
    if ((!started[i]) && (startDelay[i] == 0)) {
      started[i] = true;
      part[i]+=startPart;
      index[i] = partIndices[part[i]];
      setNextNote(i);
    } 
    else {
      startDelay[i]--;
    }

    if ((started[i]) && (!finished[i])) {
      if (playNote[i]) {
        synth.allNotesOff(i);
        synth.noteOn(i, note[i], 127);
        playNote[i] = false;
      } 
      beat[i]++;
      noteCount[i]--;
      if (noteCount[i] == 0) {
        if (beat[i] >= partBeats[part[i]]) {
          beat[i] = 0;

          // Add a spread to hold back the lead if it gets too far ahead
          if ((max-part[i] > 4)) {
            chance = 50;
          } 
          else {
            chance = 90;
          }

          if ((max == part[i]) && (spread > 4)) {
            chance = 99;
          }

          if ((random(1, 100) > chance)) {

            part[i]++;
            if ((i==9) || (random(1, 100)>90)) {
              synth.setChannelVolume(i, 0);
            } 
            else {
              synth.setChannelVolume(i, volume[i]);
            }   
            max = 0;
            spread=0;
            for (int j=0; j<numInstruments; j++) {

              if (part[j]> max) { 
                max = part[j]; 
              }
              if ((max-part[j])>spread) {
                spread = max-part[j];
              }
              //  Serial.print(part[j]);
              // Serial.print(" ");
            }
            //  Serial.println();
            if (part[i]>numParts) {
              //Serial.print("!");
              finished[i] = true;
              numFinished++;
            }
          }
          index[i] = partIndices[part[i]];
        } 
        else {
          index[i] = index[i]+2;
        }

        // TODO: Add flocking volume changes

        setNextNote(i);
      }
    } 
  }

  delay(tempo-(millis()-startTime));

}

void pan(int channel, int value) {
  // TODO: Add this to library
  byte command[3] = { 
    (0xb0 | (channel & 0x0f)), 0x0A, (value)   };
  synth.fluxWrite(command, 3);
}


