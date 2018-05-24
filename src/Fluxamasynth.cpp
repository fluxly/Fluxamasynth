/*  
-------------------------------------------------------
Fluxamasynth.cpp

A library for using the Modern Device Fluxamasynth instrument building and generative music platform.
More info is at www.moderndevice.com/fluxamasynth

-------------------------------------
This software is in the public domain.
Modified 4/2011 R.McGinnis
Updated 2018 Modern Device
------------------------------------------------------- 
*/

#include <Arduino.h>
#include "Fluxamasynth.h"

#if (FS_PLATFORM == FLUXAMASYNTH_SHIELD)
                                                       
Fluxamasynth::Fluxamasynth() : synth(255, 4) {      
    synth.begin(31250);
}

Fluxamasynth::Fluxamasynth(byte rxPin, byte txPin) : synth(rxPin, txPin) {
    synth.begin(31250);
}

size_t Fluxamasynth::fluxWrite(byte c) {
    synth.write(c);
}

size_t Fluxamasynth::fluxWrite(byte *buf, int cnt) {
    for (int i=0; i<cnt; i++) {
        synth.write(buf[i]);
    }
}
#endif

#if (FS_PLATFORM == FLUXAMASYNTH_ESP32) 
#include <HardwareSerial.h>

HardwareSerial synth32(2);

Fluxamasynth::Fluxamasynth() {  
    synth32.begin(31250);
	while (!synth32);
}

size_t Fluxamasynth::fluxWrite(byte c) {
    synth32.write(c);
}

size_t Fluxamasynth::fluxWrite(byte *buf, int cnt) {
    for (int i=0; i<cnt; i++) {
        synth32.write(buf[i]);
    }
}

#endif

#if (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER) 

Fluxamasynth::Fluxamasynth() {  
    Serial1.begin(31250);
}

size_t Fluxamasynth::fluxWrite(byte c) {
    Serial1.write(c);
}

size_t Fluxamasynth::fluxWrite(byte *buf, int cnt) {
    for (int i=0; i<cnt; i++) {
        Serial1.write(buf[i]);
    }
}
#endif


void Fluxamasynth::noteOn(byte channel, byte pitch, byte velocity) {
    byte command[3] = { 0x90 | (channel & 0x0f), pitch, velocity };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::noteOff(byte channel, byte pitch) {
    byte command[3] = { 0x80 | (channel & 0x0f), pitch, byte(0x00) };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::programChange(byte bank, byte channel, byte v) {
    // bank is either 0 or 127
    byte command[3] = { 0xB0 | (channel & 0x0f), byte(0x00), bank };
    this->fluxWrite(command, 3);
    this->fluxWrite(0xc0 | (channel & 0x0f));
    this->fluxWrite(v);
}

void Fluxamasynth::pitchBend(byte channel, int v) {
    // v is a value from 0 to 1023
    // it is mapped to the full range 0 to 0x3fff
    v = map(v, 0, 1023, 0, 0x3fff);
    byte command[3] = { 0xe0 | (channel & 0x0f), byte(v & 0x00ef), byte(v >> 7) };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::pitchBendRange(byte channel, byte v) {
    // Also called pitch bend sensitivity
    //BnH 65H 00H 64H 00H 06H vv
    byte command[7] = {0xb0 | (channel & 0x0f), 0x65, 0x00, 0x64, 0x00, 0x06, (v & 0x7f)};
    this->fluxWrite(command, 7);
}

void Fluxamasynth::midiReset() {
    this->fluxWrite(0xff);
}

void Fluxamasynth::setChannelVolume(byte channel, byte level) {
    byte command[3] = { (0xb0 | (channel & 0x0f)), 0x07, level };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::allNotesOff(byte channel) {
    // BnH 7BH 00H
    byte command[3] = { (0xb0 | (channel & 0x0f)), 0x7b, 0x00 };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::setMasterVolume(byte level) {
    //F0H 7FH 7FH 04H 01H 00H ll F7H
    byte command[8] = { 0xf0, 0x7f, 0x7f, 0x04, 0x01, 0x00, (level & 0x7f), 0xf7 };
    this->fluxWrite(command, 8);
}

void Fluxamasynth::setReverb(byte channel, byte program, byte level, byte delayFeedback) {
    // Program 
    // 0: Room1   1: Room2    2: Room3 
    // 3: Hall1   4: Hall2    5: Plate
    // 6: Delay   7: Pan delay
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x50);
    this->fluxWrite(program & 0x07);
 
    // Set send level
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x5b);
    this->fluxWrite(level & 0x7f);
  
    if (delayFeedback > 0) {
      //F0H 41H 00H 42H 12H 40H 01H 35H vv xx F7H
      byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x35, (delayFeedback & 0x7f), 0xf7 };
      this->fluxWrite(command, 11);
    }
}

void Fluxamasynth::setChorus(byte channel, byte program, byte level, byte feedback, byte chorusDelay) {
    // Program 
    // 0: Chorus1   1: Chorus2    2: Chorus3 
    // 3: Chorus4   4: Feedback   5: Flanger
    // 6: Short delay   7: FB delay
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x51);
    this->fluxWrite(program & 0x07);
 
    // Set send level
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x5d);
    this->fluxWrite(level & 0x7f);
  
    if (feedback > 0) {
    //F0H 41H 00H 42H 12H 40H 01H 3BH vv xx F7H
	byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x3B, (feedback & 0x7f), 0xf7 };
	this->fluxWrite(command, 11);
    }
  
    if (chorusDelay > 0) {
    // F0H 41H 00H 42H 12H 40H 01H 3CH vv xx F7H
        byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x3C, (chorusDelay & 0x7f), 0xf7 };
	this->fluxWrite(command, 11);
    }
}
