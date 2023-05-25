/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "include/sb_defs.h"

static volatile uint8_t m_Command = 0x00;
static volatile uint8_t m_Bits = 0x00;

typedef enum {
	newByte_eEMPTY = 0x00,
	newByte_eNEW
} newByte_e;

static volatile newByte_e m_NewByte = newByte_eEMPTY;

// Timer0 compare interrupt - this is called once, to ommit MIDI start bit
ISR(TIM0_COMPA_vect) {
  	TIMSK &= ~(1<<OCIE0A);
  	TCNT0  = 0x00;
  	OCR0A  = 0xff;

	if(m_Bits == 0x00) {
  		USICR = 0x01 << USIOIE | 0x00 << USIWM0 | 0x01 << USICS0;
  		USISR = 0x01 << USIOIF | 0x08;
	}

	m_Bits++;
	//if(m_Bits < 0x08) TIMSK |= 1<<OCIE0A;
}

ISR (PCINT0_vect) {
  if (!(PINB & 0x01 << PINB0)) {
    GIMSK &= ~(1<<PCIE);

    TCNT0  = 0x00;
    OCR0A  = 0x20; // Delay (51+1)*8 cycles
    TIFR  |= 0x01 << OCF0A;
    TIMSK |= 0x01 << OCIE0A;
  }

}

ISR (USI_OVF_vect) {
  USICR = 0x00;

  m_Command = USIBR;
  m_NewByte = newByte_eNEW;
}

uint8_t  SB_MIDIUSI_SampleByte(void) {
	m_NewByte = newByte_eEMPTY;
	m_Bits    = 0x00;

	PCMSK |= 0x01 << PCINT0;

	GIFR |= 1<<PCIF;
  	GIMSK |= 1<<PCIE;

	while(m_NewByte == newByte_eEMPTY);

	return m_Command;
}
