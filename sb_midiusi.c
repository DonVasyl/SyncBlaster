/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "include/sb_defs.h"

static volatile uint8_t m_DataByte = 0x00;

typedef enum {
	newByte_eEMPTY = 0x00,
	newByte_eNEW
} newByte_e;

// USI status variable
static volatile newByte_e m_NewByte = newByte_eEMPTY;

// Timer/Counter0 compare interrupt handler - this is called once, to ommit MIDI start bit
ISR(TIM0_COMPA_vect) {
	// setting Timer/Counter0 for regular operation after passing the start bit
	// disabling 
  	TIMSK  &= ~(1<<OCIE0A);
  	TCNT0   = 0x00;
  	OCR0A   = 0xff;

	// start catching eight significant bits until USI Overflow, right after start bit
	USICR   = 0x01 << USIOIE | 0x00 << USIWM0 | 0x01 << USICS0;
	USISR   = 0x01 << USIOIF | 0x08;
}

// Pin Change Interrupt handler
ISR (PCINT0_vect) {
	if (!(PINB & 0x01 << PINB0)) {
		// disable Pin Change Interrupt
		GIMSK  &= ~(1<<PCIE);

		// start Timer/Counter0, count up to 0x20, then trigger Compare Match A interrupt
		TCNT0   = 0x00;
		OCR0A   = 0x20;
		TIFR   |= 0x01 << OCF0A;
		TIMSK  |= 0x01 << OCIE0A;
	}
}

ISR (USI_OVF_vect) {
	// disable USI
	USICR   = 0x00;

	m_DataByte = USIBR;
	m_NewByte = newByte_eNEW;
}

void SB_MIDIUSI_Init(void) {
	// disable USI
	USICR   = 0x00;

	// Timer/Counter0 init (USI-triggering timer)

	// disable Timer/Counter0
	TCCR0A  = 0x00;
	TCCR0B  = 0x00;
	// reset Timer/Counter0
	TCNT0   = 0x00;

	GTCCR  |= _BV(PSR0); // Reset prescaler of Timer/Counter0 

	// set mode of operation
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00);
}

uint8_t  SB_MIDIUSI_SampleByte(void) {
	m_NewByte = newByte_eEMPTY;

	// enable Pin Change Interrupt for pin 0
	PCMSK  |= 0x01 << PCINT0;

	// reset Pin Change Interrupt flag and enable Pin Change Interrupt
	GIFR   |= 1<<PCIF;
	GIMSK  |= 1<<PCIE;

	// wait for new byte arrival
	while(m_NewByte == newByte_eEMPTY);

	return m_DataByte;
}
