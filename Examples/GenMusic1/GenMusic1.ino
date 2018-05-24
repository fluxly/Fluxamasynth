#include <Fluxamasynth.h>
#include <PgmChange.h>

Fluxamasynth synth = Fluxamasynth();

#define numInstruments 4

byte parts[4];
int measures[16];
int notes[16];
int rhythm[16];
int percussion[16];
char partName[4] = {'a','b','c','d'};
int noteName[16];
int binaryChoice[16];
int noteOn[4] = {0, 0, 0, 0};
int rhythmOn[4] = {0, 0, 0, 0};
int tempo=30;

// Limit instrument choice
int instrument[numInstruments] = {13, 13, 13, 13};

void setup() {
  randomSeed(millis()+analogRead(0));
  Serial.begin(9600);
  chooseParts();
  synth.setMasterVolume(85);
  for (int i=0; i<numInstruments; i++) {
    synth.programChange(0, i, instrument[i]);
    synth.pan(i,127/numInstruments*i);
    //synth.setChannelVolume(i, volume[i]);
    synth.setReverb(i, 3, 127, 25);
  }
  synth.setReverb(9,5,255,100);   //Plate reverb
  pinMode(18, INPUT);
  delay(2000);
  
}

void chooseParts() {
  parts[0] = random(254)+1;
  parts[1] = random(255);
  parts[1] = parts[1] & (parts[1]^parts[0]);
  parts[2] = random(255);
  parts[2] = parts[2] & (parts[2]^(parts[0]|parts[1]));
  parts[3] = ~(parts[0]|parts[1]|parts[2]);
}

void chooseMeasures(int p) {
  measures[0+p] = random(254)+1;
  measures[1+p] = random(255);
  measures[1+p] = measures[1+p] & (measures[1+p]^measures[0+p]);
  measures[2+p] = random(255);
  measures[2+p] = measures[2+p] & (measures[2+p]^(measures[0+p]|measures[1+p]));
  measures[3+p] = ~(measures[0+p]|measures[1+p]|measures[2+p]);
}

void chooseNotes(int p) {
  int baseNote = random(40)+50;
  noteName[p] = baseNote;
  noteName[p+1]=baseNote +  (random(3)-1)*7;
  noteName[p+2]=baseNote +  (random(3)-1)*4 + random(3)-1;
  noteName[p+3]=baseNote + (random(3)-1)*random(8);
}

void chooseRhythm(int p) {
  rhythm[0+p] = random(254)+1;
  rhythm[1+p] = random(255);
  rhythm[1+p] = rhythm[1+p] & (rhythm[1+p]^rhythm[0+p]);
  rhythm[2+p] = random(255);
  rhythm[2+p] = rhythm[2+p] & (rhythm[2+p]^(rhythm[0+p]|rhythm[1+p]));
  rhythm[3+p] = ~(rhythm[0+p]|rhythm[1+p]|rhythm[2+p]);
}

void choosePercussion(int p) {
  //percussion[p] = 36;
  //percussion[p+1]=38;
  //percussion[p+2]=42;
  //percussion[p+3]=44;
  percussion[p] = random(127);
  percussion[p+1]=random(127);
  percussion[p+2]=random(127);
  percussion[p+3]=random(127);
}

void loop() {
 // pinMode(12, INPUT);
 // while(digitalRead(12) == LOW) {
    //wait
 // }
 // pinMode(12, OUTPUT);
 // digitalWrite(12, LOW);
  
  // New Song
  for (int i=0; i<numInstruments; i++) {
    synth.allNotesOff(i);
  }
  
  tempo = random(200)+10;
  Serial.println();
  Serial.println("New Song!");
  for (int i=0; i<4; i++) {
        instrument[i] = random(127);
        synth.programChange(0, i, instrument[i]);
  }

  int numSections = random(20)+60;
  for (int m=0; m<4; m++) {
    choosePercussion(m);
  }
 
  for (int sections=0; sections<numSections; sections++) {
    // repeat random number of times
    chooseParts();

    for (int m=0; m<4; m++) {
       chooseMeasures(m);
       chooseNotes(m);
       chooseRhythm(m);
    }
    
    for (int i=0; i<16; i++) {
      if (random(100)>50) {
        binaryChoice[i] = 1;
      } else {
        binaryChoice[i] = 0;
      }
    }
    tempo = tempo-random(tempo/160);

//Add four time channels: triplet, quad, quintuplet, septuplet
//

    while(random(100)<10) {
      for (int i=0; i<8; i++) {
      Serial.println();
      for (int part =0; part<4; part++) {
        if ((parts[part]>>i) & 0x01) {
          Serial.print(partName[part]);
          for (int beat =0; beat<8; beat++) {
            delay(tempo);
            //Serial.print("|");
            for (int voice=0; voice<4; voice++) {
              if (((measures[part+voice]>>beat) & 0x01)==binaryChoice[part+voice]) {
                if (noteOn[voice]!=noteName[part+voice]) {
                  if (noteOn[voice]==0) {
                    Serial.print(voice);
                    synth.noteOn(voice, noteName[part+voice], 127);
                    noteOn[voice] = noteName[part+voice];
                  } else {
                    synth.noteOff(voice, noteOn[voice]);
                    synth.noteOn(voice, noteName[part+voice], 127);
                    //Serial.print("PrevOff");
                    //Serial.print("On");
                    noteOn[voice] = noteName[part+voice];
                  }
                } else {
                    //Serial.print("Hold");
                }
               // Serial.print(voice);
                //Serial.print(": ");
                //Serial.print(noteName[part+voice]);
                //Serial.print(" ");
              } else {
                if (noteOn[voice]) {
                  synth.noteOff(voice, noteOn[voice]);
                  noteOn[voice]=0;
                  //Serial.print(voice);
                  // Serial.print(": off");
                }
              }
              if (((rhythm[part+voice]>>beat) & 0x01)==binaryChoice[part+voice]) {
                if (!rhythmOn[voice]) {
                  rhythmOn[voice]=1;
                  synth.noteOn(9, percussion[voice], 100) ;
                  synth.noteOff(9, percussion[voice]) ;
                } 
              } else {
                rhythmOn[voice]=0;
              }
            }
          }
        }
      }
    }
  }
  }
  
   
   delay(2000);
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
  pinMode(12, INPUT);
}

