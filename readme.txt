Fluxamasynth_NSS is a drop-in replacement for Modern Devices' Fluxamasynth library, but requiring that the NewSoftSerial library be provided to do serial communication (tx).

The default constructor (Fluxamasynth()) defaults to pin 4 for tx and 255 for rx (255 -> unused by NSS)
An explicit constructor (Fluxamasynth(byte rxPin, byte txPin)) is also provided.

A polymorphic straight-thru write (fluxWrite(...) has been added, with the caveat that the synth must be left in a ready state for other library calls.  This is to say that MIDI/control sequences must be written in total, before a different library call is issued.

    void fluxWrite(byte c);
    void fluxWrite(byte *buf, int cnt);

The Fluxamasynth library has been released to the public domain.  This version is similarly released to the public domain.