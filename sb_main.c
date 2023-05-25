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

	// disable USI
	USICR = 0x00;

	// Timer0 init (USI-triggering timer)
	TCCR0A  = 0x00;
  	TCCR0B  = 0x00;
 	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00);

	// Timer1 init (delay timer)
 	TCCR1 = 0x00; // Stop timer

 	GTCCR |= _BV(PSR1) | _BV(PSR0); // Reset prescalers of Timer/Counter1 
  	OCR1A = 0; // T = prescaler / 1MHz = 0.004096s; OCR1A = (1s/T) - 1 = 243
  	OCR1C = 255; //

	TCCR1 |= _BV( CS13 ) | _BV( CS11 ) | _BV( CS10 );

	TCNT0 = 0;
	TCNT1 = 0;

	TIMSK |= ( 1 << TOIE1 ); // enable Timer/Counter1 Overflow Interrupt

	power_adc_disable();

	sei();

#ifdef SB_TESTING
	SB_DEBUG_BlinkMIDIByte(&m_DEBUG_Buffy);
#endif

    while (1) {
		SB_MIDIUSI_SampleByte();

		if(USIBR == SB_MIDI_START) {
			SB_TASCAM_Record();
		}

		if(USIBR == 0x3F) { // FC
			SB_TASCAM_StopAndRewind();
		}
	}
}
