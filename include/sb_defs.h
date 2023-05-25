/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

// MIDI SysRT messages in reverse bit order

// Start - 0xFA
#define SB_MIDI_START      0x5F

// Continue - 0xFB
#define SB_MIDI_CONTINUE   0xDF

// Stop - 0xFC
#define SB_MIDI_STOP       0x3F

// MCU IO configuration
#define SB_PIN_MIDI_IN PB0
#define SB_PIN_REC     PB1
#define SB_PIN_PLAY    PB2
#define SB_PIN_STOP    PB3
#define SB_PIN_REW     PB4
// PB5 will be SYNC IN digital trigger with HVP

// TASCAM control sequences for CD4066 IC
// RECORD AND PLAY, RUMIA version
#define SB_TASCAM_DELAY_RECORD_PRE  2
#define SB_TASCAM_DELAY_RECORD_IN   2
#define SB_TASCAM_DELAY_RECORD_POST 2

// STOP AND REWIND, final sequence
#define SB_TASCAM_DELAY_REWIND_PRE  2
#define SB_TASCAM_DELAY_REWIND_IN   1
#define SB_TASCAM_DELAY_REWIND_POST 0

// Serial byte display
#define SB_DEBUG_DELAY_BIT_STEP     8
#define SB_DEBUG_DELAY_STOP_LIGHT   3
#define SB_DEBUG_DELAY_STOP_SHADOW  2
#define SB_DEBUG_DELAY_EACH_STEP    8

