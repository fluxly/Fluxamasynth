/* 
*  FluxamasynthBendingNotes
*  Demonstrates pitch bending on the Modern Device Fluxamasynth Shield
*  modernDevice.com/fluxamasynth
*/

#include "Fluxamasynth.h"

Fluxamasynth synth;

int i = 512;                   // middle of pitch bend range

void setup()
{
  // Serial1.begin(31250);     // Uncomment if using a Feather M0 board
  synth.midiReset();
  synth.programChange(127, 0, 91);
  synth.pitchBendRange(0, 4);  //double the pitch bend sensitivity (4 semitones)

  synth.noteOn(0, 62, 127);
}

void loop()
{
  while (i < 1023)             // loop to upper bound of pitch bend range
  {
    synth.pitchBend(0, i);
    i+= 16;
    delay(10);
  }

  while (i > 0)                // loop to lower bound of pitch bend range
  {
    synth.pitchBend(0, i);
    i-= 16;
    delay(10);
  }
}
