/* FluxamasynthCrossFading.pde
* Demonstrates cross fading on the Modern Device Fluxamasynth Shield
* ModernDevice.com
*/

#include "Fluxamasynth.h"
Fluxamasynth synth;

int tone1 = 53;
int tone2 = 61;

int vol1 = 64;                         // volumes must be inverses of eachother
int vol2 = 127 - vol1;                 // or they'll get out of sync

int i = 1;                             // step size
int dir = 1;                           // direction to step in: 1 = up, -1 = down

void setup() {

  synth.programChange(0, 0, 90);       // give our two channels different voices
  synth.programChange(0, 1, 89);

  synth.setChannelVolume(0, vol1);     // set their volumes
  synth.setChannelVolume(1, vol2);

  synth.noteOn(0, tone1, 127);         // turn on a note for each channel
  synth.noteOn(1, tone2, 127);
}

void loop() {
  while (vol1 > i && vol1 < (127 - i)) // loops while in valid volume range
  {
    delay(20);
    vol1+= i * dir;                    // vol1 increments by i in direction dir
    vol2 = 127 - vol1;                 // vol2 is always the inverse
    synth.setChannelVolume(0, vol1);   // update volumes
    synth.setChannelVolume(1, vol2);
  }
  dir*= -1;                            // once volumes reach bounds, reverse dir
  vol1+= i * dir;                      // increment once in new direction
  int vol2 = 127 - vol1;               // to ensure volumes are back in range
}
