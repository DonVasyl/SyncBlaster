/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

// *** Macross ***

#define SB_MIDIUSI_REVERSE_8_BITS(iByte)  \
    (((iByte & 0x01) << 0x07) | \
     ((iByte & 0x02) << 0x05) | \
     ((iByte & 0x04) << 0x03) | \
     ((iByte & 0x08) << 0x01) | \
     ((iByte & 0x10) >> 0x01) | \
     ((iByte & 0x20) >> 0x03) | \
     ((iByte & 0x40) >> 0x05) | \
     ((iByte & 0x80) >> 0x07))

#define SB_MIDIUSI_COMPOSE_OP_AND_CHANNEL(iOp, iChannel) \
    (((iChannel & 0x01) << 0x07) | \
     ((iChannel & 0x02) << 0x05) | \
     ((iChannel & 0x04) << 0x03) | \
     ((iChannel & 0x08) << 0x01) | \
     ((iOp & 0x01) << 0x03) | \
     ((iOp & 0x02) << 0x01) | \
     ((iOp & 0x04) >> 0x01) | \
     ((iOp & 0x08) >> 0x03))

// *** MIDI ***

// MIDI SysRT

// Start
#define SB_MIDI_START                0xFA

// Continue
#define SB_MIDI_CONTINUE             0xFB

// Stop
#define SB_MIDI_STOP                 0xFC

// MIDI Channel messages

// Note On
#define SB_MIDI_NOTE_ON              0x09

// Note Off
#define SB_MIDI_NOTE_OFF             0x08

// *** I/O configuration ***

// MIDI input channel
#define SB_MIDI_IN_CHANNEL           0x06

// MIDI note to:
// * act always as [Start] when [Note On]
// * and as [Stop] when [Note Off] if SB_SINGLE_NOTE_GATE_MODE macro is defined
#define SM_MIDI_NOTE_START_GATE_STOP 0x0c

// MIDI note to act as Stop when Note On
#define SM_MIDI_NOTE_STOP            0x0e

// MCU IO configuration
#define SB_PIN_MIDI_IN               PB0
#define SB_PIN_REC                   PB1
#define SB_PIN_PLAY                  PB2
#define SB_PIN_STOP                  PB3
#define SB_PIN_REW                   PB4
// PB5 will be SYNC IN digital trigger with HVP

// *** Timigs ***

// Serial byte display
#define SB_DEBUG_DELAY_BIT_STEP      8
#define SB_DEBUG_DELAY_STOP_LIGHT    3   
#define SB_DEBUG_DELAY_STOP_SHADOW   2
#define SB_DEBUG_DELAY_EACH_STEP     8

// ** TASCAM control sequences for CD4066 IC **

// RECORD AND PLAY, RUMIA version
#define SB_TASCAM_DELAY_RECORD_PRE   2
#define SB_TASCAM_DELAY_RECORD_IN    2
#define SB_TASCAM_DELAY_RECORD_POST  0

// STOP AND REWIND, final sequence
#define SB_TASCAM_DELAY_REWIND_PRE   14
#define SB_TASCAM_DELAY_REWIND_IN    8
#define SB_TASCAM_DELAY_REWIND_POST  0
