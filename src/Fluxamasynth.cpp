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
#pragma once

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

#if (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER)  || (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER_M0)

Fluxamasynth::Fluxamasynth() {  
#if (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER)
	Serial1.begin(31250);
#endif
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
      byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x35, (delayFeedback & 0x7f), 0x00, 0xf7 };
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
	byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x3B, (feedback & 0x7f), 0x00, 0xf7 };
	this->fluxWrite(command, 11);
    }
  
    if (chorusDelay > 0) {
    // F0H 41H 00H 42H 12H 40H 01H 3CH vv xx F7H
    byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x3C, (chorusDelay & 0x7f), 0x00, 0xf7 };
	this->fluxWrite(command, 11);
    }
}

void Fluxamasynth::pan(int channel, int value) {
  byte command[3] = {
    (0xb0 | (channel & 0x0f)), 0x0A, (value)   };
  this->fluxWrite(command, 3);
}

void Fluxamasynth::setEQ(byte channel, byte lowBand, byte medLowBand, byte medHighBand, byte highBand,
           byte lowFreq, byte medLowFreq, byte medHighFreq, byte highFreq) {
    //BnH 63H 37H 62H 00H 06H vv   low band
    //BnH 63H 37H 62H 01H 06H vv   medium low band
    //BnH 63H 37H 62H 02H 06H vv   medium high band
    //BnH 63H 37H 62H 03H 06H vv   high band
    //BnH 63H 37H 62H 08H 06H vv   low freq
    //BnH 63H 37H 62H 09H 06H vv   medium low freq
    //BnH 63H 37H 62H 0AH 06H vv   medium high freq
    //BnH 63H 37H 62H 0BH 06H vv   high freq
    byte command[7] = {0xb0 | (channel & 0x0f), 0x63, 0x37, 0x62, 0x00, 0x06, (lowBand & 0x7f)};
    this->fluxWrite(command, 7);
    command[6] = (lowBand & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (medLowBand & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (medHighBand & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (highBand & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (lowFreq & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (medLowFreq & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (medHighFreq & 0x7f);
    this->fluxWrite(command, 7);
    command[6] = (highFreq & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setTuning(byte channel, byte coarse, byte fine) {
    // This will turn off any note playing on the channel
    //BnH 65H 00H 64H 01H 06H vv  Fine
    //BnH 65H 00H 64H 02H 06H vv  Coarse
    byte command[7] = {0xb0 | (channel & 0x0f), 0x65, 0x00, 0x64, 0x01, 0x06, (fine & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x02;
    command[6] = (coarse & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setVibrate(byte channel, byte rate, byte depth, byte mod) {
    //BnH 63H 01H 62H 08H 06H vv  Rate
    //BnH 63H 01H 62H 09H 06H vv  Depth
    //BnH 63H 01H 62H 0AH 06H vv  Delay modify
    byte command[7] = {0xb0 | (channel & 0x0f), 0x63, 0x01, 0x62, 0x08, 0x06, (rate & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x09;
    command[6] = (depth & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x0A;
    command[6] = (mod & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setTVF(byte channel, byte cutoff, byte resonance) {
    //BnH 63H 01H 62H 20H 06H vv  Cutoff
    //BnH 63H 01H 62H 21H 06H vv  Resonance
    byte command[7] = {0xb0 | (channel & 0x0f), 0x63, 0x01, 0x62, 0x20, 0x06, (cutoff & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x21;
    command[6] = (resonance & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setEnvelope(byte channel, byte attack, byte decay, byte release) {
    //BnH 63H 01H 62H 63H 06H vv
    //BnH 63H 01H 62H 64H 06H vv
    //BnH 63H 01H 62H 66H 06H vv
    byte command[7] = {0xb0 | (channel & 0x0f), 0x63, 0x01, 0x62, 0x63, 0x06, (attack & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x64;
    command[6] = (decay & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x66;
    command[6] = (release & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setScaleTuning(byte channel, byte v1, byte v2, byte v3, byte v4, byte v5, byte v6,
                                  byte v7, byte v8, byte v9, byte v10, byte v11, byte v12) {
    //F0h 41h 00h 42h 12h 40h 1nh 40h v1 v2 v3 ... v12 F7h
    // values are in range 00h = -64 cents to 7fh = +64 cents, center is 40h
    byte command[21] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x10 | (channel & 0x0f), 0x40,
        v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, 0xf7 };
    this->fluxWrite(command, 21);
}

void Fluxamasynth::setModWheel(byte channel, byte pitch, byte tvtCutoff, byte amplitude, byte rate, byte pitchDepth, byte tvfDepth, byte tvaDepth) {
    //F0h 41h 00h 42h 12h 40h 2nh 00h vv xx F7h
    byte command[11] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x20 | (channel & 0x0f), 0x00, pitch, 0x00, 0xf7 };
    this->fluxWrite(command, 11);
    command[8] = 0x01;
    command[9] = tvtCutoff;
    this->fluxWrite(command, 11);
    command[8] = 0x02;
    command[9] = amplitude;
    this->fluxWrite(command, 11);
    command[8] = 0x03;
    command[9] = rate;
    this->fluxWrite(command, 11);
    command[8] = 0x04;
    command[9] = pitchDepth;
    this->fluxWrite(command, 11);
    command[8] = 0x05;
    command[9] = tvfDepth;
    this->fluxWrite(command, 11);
    command[8] = 0x06;
    command[9] = tvaDepth;
    this->fluxWrite(command, 11);
}


