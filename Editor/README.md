# 60Knobs Editor

The editor comes in two versions: As tested MaxMSP patch and beta state app for Linux, MacOS and Windows.

## Max MSP

Exported as standalone application using Cycling74 Max6 Runtime, is available for Win and OSX under the voice links at:
http://www.bastl-instruments.com/instruments/sixtyknobs/

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

## Standalone App

The app is still in beta state. Feel free to test it.

| Operating System | Supported Versions |
| -----------------| -------------------|
| Linux | Compiled on Debian Stretch. Older versions on request |
| MacOS | Compiled on High Sierra 10.13.1 |
| Windows | Compiled on Windows 7 |
