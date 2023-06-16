# SyncBlaster
### MIDI input interface for TASCAM DP00x series of multitrack recorders
### based on Microchip ATtiny25/45/85 MCU

![SyncBlaster prototype rev 3 schematics drawn in KiCAD](https://github.com/DonVasyl/SyncBlaster/blob/assets/images/sync_schematics_rev3.png?raw=true)

## Main features:
* Opto-coupled MIDI input
* Firmware image below 1024 bytes
* Recognition of SysRT (Start, Stop) and Channel (Note On, Note Off) MIDI messages
* Simulation of two key sequences: one to start recording and one to stop and rewind to zero time
* Four keys of recorder's keyboard are bypassed by external CMOS IC
* 10 wires have to be soldered to recorder's PCBs

## Operation principles:
SyncBlaster firmware operates on wide range of Microchip ATtiny microcontrollers, equiped with small amounts of memory and no UART peripheral. Instead, USI peripheral provides
partial hardware support for serial communicaton (shift register with overflow interrupt), howver time measurement must be provided externally, possibly by usage of Timer/Counter peripherals.
Four of the remaining output pins of the microcontroller are used to control CD4066 IC (quad bilateral switch), acting as recorder's keyboard bypass. Available key sequences

* Record + Play
* Stop + Rewind

are intended to initialize recording session on particular track(s) and to stop and immediately rewind virtual tape timing to zero. Automated start of recording process,
with as small latency as possible, enables recording of multiple tracks in series of takes, and eliminates manual synchronisation of recorded material between recorder's tracks.
Delay values of key sequences have been extensively tested against two main requirements:

* Quick & certain start of the recording process, right after reception of proper MIDI message;
* Certain stop and rewind operation, with higher latency latitude.

All delay values are reflecting recorder's characteristic, please note that recorder's responsiveness may vary in particular recording stages and states.

## MIDI implementation:
By default, firmware receives Channel MIDI messages on 7th channel. There are three scenarios of MIDI recording control:

1. SysRT: Start (0xFA) -> Stop (0xFC)
2. Channel: Note On (0x96 0x0C) -> Note On (0x96 0x0E)
3. Channel in single note gate mode: Note ON (0x96 0x0C) -> Note Off (0x86 0x0C)

Standard firmware builds supports scenario 1 and 2. Third scenario requires building firmware image with "singlenote" part in build target name. Please note, that scenario 3. excludes scenario 2.
This special scenario requires single note key to be kept pressed during recording process; it has been designed for usage with customized gear (i.e. with added mechanical switch(es), bypassing note key(s)).

MIDI compatibility of SyncBlaster have been verified with variety of equipment from different vendors (Yamaha, Behringer, KORG, ...).

## Missing features / TODO:
* SYNC IN digital input support (HV programmer integration required)
* Expanding range of equipment vendors & tested device types (i.e Novation, ...)

by Mateusz Maciejewski ^ TOXIC_DEATH.117