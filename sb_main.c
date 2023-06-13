/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>	
#include <avr/power.h>

#include <stdint.h>

#include "include/sb_defs.h"
#include "include/sb_delay.h"
#include "include/sb_midiusi.h"
#include "include/sb_tascam.h"

#ifdef SB_TESTING
#include "include/sb_debug.h"
#endif

#ifdef SB_TESTING
volatile static uint8_t m_DEBUG_Buffy = 0xEA;
#endif

int main(void) {
	// configure I/O lines
	DDRB = 0b00011110; 
	PORTB = 0b00000000;

	// initialize subsystems
	SB_DELAY_Init();
	SB_MIDIUSI_Init();

	power_adc_disable(); // save some electric power

	sei(); // enable interrupts

#ifdef SB_TESTING
	SB_DEBUG_BlinkMIDIByte(&m_DEBUG_Buffy);
#endif

    while (1) {
		switch(SB_MIDIUSI_SampleByte()) {
			case SB_MIDIUSI_REVERSE_8_BITS(SB_MIDI_START):
				SB_TASCAM_Record();
				break;

			case SB_MIDIUSI_REVERSE_8_BITS(SB_MIDI_STOP):
				SB_TASCAM_StopAndRewind();
				break;

			case SB_MIDIUSI_COMPOSE_OP_AND_CHANNEL(SB_MIDI_NOTE_ON, SB_MIDI_IN_CHANNEL):
				switch(SB_MIDIUSI_SampleByte()) {
				case SB_MIDIUSI_REVERSE_8_BITS(SM_MIDI_NOTE_START_GATE_STOP):
					SB_TASCAM_Record();
					break;

#ifndef SB_SINGLE_NOTE_GATE_MODE	
				case SB_MIDIUSI_REVERSE_8_BITS(SM_MIDI_NOTE_STOP):
					SB_TASCAM_StopAndRewind();
					break;
#endif

				default:
					continue;

				}
				break;
#ifdef SB_SINGLE_NOTE_GATE_MODE	
			case SB_MIDIUSI_COMPOSE_OP_AND_CHANNEL(SB_MIDI_NOTE_OFF, SB_MIDI_IN_CHANNEL):
				switch(SB_MIDIUSI_SampleByte()) {
				case SB_MIDIUSI_REVERSE_8_BITS(SM_MIDI_NOTE_START_GATE_STOP):
					SB_TASCAM_StopAndRewind();
					break;
				
				default:
					continue;

				}
				break;
#endif

			default:
				continue;

		}
	}
}
