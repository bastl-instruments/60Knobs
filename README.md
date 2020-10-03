# 60Knobs
Firmware, Editor, Schematic, Enclosure, MIDI chart

Fork from Bastl 60Knobs - updates to the firmware to support specifying a range 
of values for the CC value range.

Some synths have a CC for say the oscillator waveform and may accept a
value from 0 to 4 or such. 

This (combined with the new version of the Editor) lets you select a new option
in the drop down list "CC Range" and inputs for min and max values.
It then works out the range of values that the knob can return to set each value.

For example :

Min 0 Max 3 - will let the first 1/4 turn of the knob send 0, 1/4 to 1/2 sends 1 etc

Editor checks in place to ensure that min can be from 0 to 126. Max from 1 to 127.
Firmware will not send a value change if the min > max for example.

Also added the "CC Range (specific Channel)" option to do the same thing but only
on the given channel.

