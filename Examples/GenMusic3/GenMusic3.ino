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
int partNotes[16] = { 
  // could be -12 also
  65, 69, 72, 53,
  58, 62, 65, 46,
  63, 67, 58, 51,
  56, 72, 63, 44
};
int tempo=100;
int timesig = 16;


// Limit instrument choice
int instrument[numInstruments] = {41, 41, 41, 41};

void setup() {
  randomSeed(millis()+analogRead(0));
  Serial.begin(9600);
  chooseParts();
  synth.setMasterVolume(100);
  for (int i=0; i<numInstruments; i++) {
    synth.programChange(0, i, instrument[i]);
    pan(i,127/numInstruments*i);
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
  measures[0+p] = random(32766)+1;
  measures[1+p] = random(32767);
  measures[1+p] = measures[1+p] & (measures[1+p]^measures[0+p]);
  measures[2+p] = random(32767);
  measures[2+p] = measures[2+p] & (measures[2+p]^(measures[0+p]|measures[1+p]));
  measures[3+p] = ~(measures[0+p]|measures[1+p]|measures[2+p]);
}

void chooseNotes(int p) {
  noteName[p] = partNotes[p]+24*random(1)-12;
  noteName[p+1]=partNotes[p+1]+24*random(1)-12;
  noteName[p+2]=partNotes[p+2]+24*random(1)-12;
  noteName[p+3]=partNotes[p+3]+24*random(1)-12;
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
 // percussion[p] = 36;
 // percussion[p+1]=38;
 // percussion[p+2]=42;
 // percussion[p+3]=44;
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
  
  tempo = 120;
  //timesig = random(9)+8;
  Serial.print("Timesig ");
  Serial.println(timesig);
    
  Serial.println();
  Serial.println("New Song!");
  for (int i=0; i<4; i++) {
        instrument[i] = 49;
        synth.programChange(0, i, instrument[i]);
  }

  int numSections = random(20)+60;
  for (int m=0; m<4; m++) {
    //choosePercussion(m);
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

//Add four tuplet channels: triplet, quad, quintuplet, septuplet
//LCD = 1/420 so triplet = 140 beats, quad = 105, quin=84, sept = 60

// Also add part length truncation

    while(random(100)<10) {
      for (int i=0; i<8; i++) {
      Serial.println();
      for (int part =0; part<4; part++) {
        if ((parts[part]>>i) & 0x01) {
          Serial.print(partName[part]);
          for (int beat =0; beat<timesig; beat++) {
            delay(tempo);
            for (int voice=0; voice<4; voice++) {
              if (((measures[part+voice]>>beat) & 0x01)==binaryChoice[part+voice]) {
                 if ((beat<timesig-3) && 
                     (((measures[part+voice+1]>>beat) & 0x01)==binaryChoice[part+voice]) &&
                     (((measures[part+voice+2]>>beat) & 0x01)==binaryChoice[part+voice]) &&
                     (((measures[part+voice+3]>>beat) & 0x01)==binaryChoice[part+voice])) {
                      Serial.print("XXXX");
                     }
                
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
                  //synth.noteOn(9, percussion[voice], 100) ;
                  //synth.noteOff(9, percussion[voice]) ;
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

void pan(int channel, int value) {
  // TODO: Add this to library
  byte command[3] = {
    (0xb0 | (channel & 0x0f)), 0x0A, (value)   };
  synth.fluxWrite(command, 3);
}