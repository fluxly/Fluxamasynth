/*  
-------------------------------------------------------
Fluxamasynth.h

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
#include "PgmChange.h"

#define FLUXAMASYNTH_ESP32       1
#define FLUXAMASYNTH_SHIELD      2
#define FLUXAMASYNTH_FOR_FEATHER 3
#define FLUXAMASYNTH_PI          4

#if defined(ESP_PLATFORM)
    #define FS_PLATFORM FLUXAMASYNTH_ESP32
  #elif defined(ARDUINO_AVR_FEATHER32U4)
    #define FS_PLATFORM FLUXAMASYNTH_FOR_FEATHER
#elif defined(ARDUINO_SAMD_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)
    #define FS_PLATFORM FLUXAMASYNTH_FOR_FEATHER_M0
  #elif defined(ARDUINO)
    #define FS_PLATFORM FLUXAMASYNTH_SHIELD
  #elif defined(RASPBERRY_PI)
    #define FS_PLATFORM FLUXAMASYNTH_PI
#endif

#ifndef FS_PLATFORM
#define FS_PLATFORM FLUXAMASYNTH_SHIELD
#endif

#if (FS_PLATFORM == FLUXAMASYNTH_SHIELD)
#include <SoftwareSerial.h>
#endif

class Fluxamasynth
{
  private:
#if (FS_PLATFORM == FLUXAMASYNTH_SHIELD)
  	SoftwareSerial synth;
#endif
	    
  public:
  
	Fluxamasynth();
#ifdef FLUXAMASYNTH_SHIELD  
    Fluxamasynth(byte rxPin, byte txPin);
#endif
    virtual size_t fluxWrite(byte c);
    virtual size_t fluxWrite(byte *buf, int cnt);
    void noteOn(byte channel, byte pitch, byte velocity);
    void noteOff(byte channel, byte pitch);
    void programChange (byte bank, byte channel, byte v);
    void pitchBend(byte channel, int v);
    void pitchBendRange(byte channel, byte v);
    void midiReset();
    void setChannelVolume(byte channel, byte level);
	void allNotesOff(byte channel);
    void setMasterVolume(byte level);
    void setReverb(byte channel, byte program, byte level, byte delayFeedback);
    void setChorus(byte channel, byte program, byte level, byte feedback, byte chorusDelay);
	void pan(int channel, int value);
    void setEQ(byte channel, byte lowBand, byte medLowBand, byte medHighBand, byte highBand,
               byte lowFreq, byte medLowFreq, byte medHighFreq, byte highFreq);
    void setTuning(byte channel, byte coarse, byte fine);
    void setVibrate(byte channel, byte rate, byte depth, byte mod);
    void setTVF(byte channel, byte cutoff, byte resonance);
    void setEnvelope(byte channel, byte attack, byte decay, byte release);
    void setScaleTuning(byte channel, byte v1, byte v2, byte v3, byte v4, byte v5, byte v6,
                        byte v7, byte v8, byte v9, byte v10, byte v11, byte v12);
    void setModWheel(byte channel, byte pitch, byte tvtCutoff, byte amplitude, byte rate, byte pitchDepth, byte tvfDepth, byte tvaDepth);
    
};
