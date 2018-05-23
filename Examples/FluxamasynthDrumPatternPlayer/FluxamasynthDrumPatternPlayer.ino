/* FluxamasynthDrumPatternPlayer.pde
* A drum machine for the Modern Device Fluxamasynth Shield
* ModernDevice.com
*
* By Michel Gutlich 26-2-2011 
* A sensor signal on analog input 0 gives some tempo dynamics
* Contact at iching@xs4al.nl
*/

#include "Fluxamasynth.h"

# define bass 36              // Define midi note numbers for several GM drum sounds
# define snare 38
# define hihatC 42
# define hihatP 44
# define hihatO 46

Fluxamasynth synth;		// create a synth object

/* **** Our drum pattern ***/
int patternSize = 16;         // A pattern of max 16 ticks ( actualy 15, because we loop and 16 is tick no. 1....you dig it ? )

// Every array cell is the velocity of the note played 
// Tick         1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
int bd  [] = {127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 90,  0}; //Bassdrum
int sn  [] = {  0,  0,  0,  0,127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}; //Snare
int hho [] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,  0}; //Hihat Open
int hhc [] = {127, 40, 80, 40,127, 40, 80, 40,127, 40, 80, 40,127,  0,  0}; //Hihat Close
int hhp [] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127}; //Hihat Pedal



// * Some basic settings */
int channel = 9;              // MIDI channel number
int tempo = 120;              // Start tempo

/*     */
int tickNo;                   // Our tick number variable

void setup() {
  Serial.begin(31250);		//  Set MIDI baud rate:

  synth.midiReset();            // Do a complete MIDI reset

  //setReverb( channel , program , level , feedback , delayFeedback )
  // Program 
  // 0: Room1   1: Room2    2: Room3 
  // 3: Hall1   4: Hall2    5: Plate
  // 6: Delay   7: Pan delay
  synth.setReverb(channel,5,255,100); // A Plate Reverb with maximum effect level

  synth.setChannelVolume(channel, 127); // max. channel volume
  synth.setMasterVolume(255);	// max. master volume
}

void loop() {
  for(int tickNo; tickNo < patternSize; tickNo++)
  {
    tempo = tempo + (random(5) - 2);  // random walk
    tempo = constrain(tempo, 40, 250);
    bassDrm(bd[tickNo]);
    snareDrm(sn[tickNo]);
    hihatClose(hhc[tickNo]);
    hihatOpen(hho[tickNo]);
    hihatPedal(hhp[tickNo]);
    tick();    
  }
} 

// Maybe it looks a bit overdone to send both velocity 0 note On and note Off information
// It saves some memory NOT to check for double note Off information...ah well..

void bassDrm (int vel )
{
 synth.noteOn(channel, bass, vel);      // play a note
 synth.noteOff(channel, bass);         // send note off
}

void snareDrm (int vel)
{
 synth.noteOn(channel, snare, vel);	// play a note
 synth.noteOff(channel, snare);         // send note off

}

void hihatClose (int vel )
{
 synth.noteOn(channel, hihatC, vel);	// play a note
 synth.noteOff(channel, hihatC);        // send note off 
}

void hihatPedal (int vel)
{
 synth.noteOn(channel, hihatP, vel);	// play a note
 synth.noteOff(channel, hihatP);        // send note off   
}

void hihatOpen (int vel )
{
 synth.noteOn(channel, hihatO, vel);	// play a note
 synth.noteOff(channel, hihatO);        // send note off 
}

void tick ()
{
 delay(tempo);
}

