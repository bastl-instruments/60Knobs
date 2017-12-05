# 60Knobs Editor

The editor comes in two versions: As MaxMSP patch and electron based standalone app for Linux, MacOS and Windows.

## Standalone App

See [Seperate Repository](https://github.com/LnnrtS/60Knobs-Editor)

## Max MSP Patches

This repository contains the separate patches of the editor:

| Patchname | Functionality |
| ----------|---------------|
| `SixtyKnobs.maxpat` | **Top level patch** |
| `CHbpatch.maxpat`  |  Global MIDI Channel settings |
| `DECODERbpatch.maxpat` | Loads preset (from text file) or lxr_default |
| `DEVICESbpatch.maxpat` | MIDI port selector |
| `ENCODERbpatch.maxpat` | Sends preset to the device or save to text file |
| `KNOBbpatch.maxpat` | Knob settings |
| `PRSTbpatch.maxpat` | Memory slot to upload selector |
| `b.Ke.maxpat` | Compiles Knob settings to SysEx format |
| `lxr_default` | device factory preset (designed for Sonic Potions LXR) |

ALL THE PATCHES ARE LICENSED AS CC-BY-SA Bastl-Instruments 2017
