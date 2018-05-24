Fluxamasynth Arduino Library

The Fluxamasynth is a programmable synthesizer in an Arduino shield format. The Arduino sends MIDI commands to the ATSAM2695 IC on the shield, which generates stereo line level audio out. The controller can be programmed using the Arduino IDE, and Modern Device has provided a library to simplify using the synthesizer.

The Fluxamasynth is based on Dream’s ATSAM2695 single-chip MIDI sound system, which is part of their Dream Sound Synthesis line of programmable ICs. The chip was intended for battery powered keyboards or portable Karaoke machines, but I’m sure you can use and abuse the Fluxamasynth in all sorts of new and interesting ways. Here are a few suggestions:

* Make your own MIDI instrument with unconventional analog sensors
* Create algorithmic compositions
* Use it in a homebrew pinball machine to generate music and sound effects
* Build an unusual percussion device
* Augment an analog instrument with a Fluxamasynth-based hyperinstrument

The Fluxamasynth has a built-in wavetable with 128 general MIDI sounds and an additional set of 128 variations and dozens of percussion sounds. It can play music in 64-voice polyphony without effects or 38 voices with effects.


See the full library reference at:

https://moderndevice.com/documentation/using-the-fluxamasynth-arduino-library/