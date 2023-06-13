/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stddef.h>

static volatile uint8_t m_Timer1 = 0x00;

ISR(TIM1_OVF_vect) {
	if(m_Timer1 > 0x00) {
		m_Timer1--;
	}

	TCNT1   = 0x00; // reset Timer/Counter1 
}

void SB_DELAY_Init(void) {
 	TCCR1   = 0x00; // stop Timer/Counter1 
	TCNT1   = 0x00; // reset Timer/Counter1 

 	GTCCR  |= _BV(PSR1); // Reset prescalers of Timer/Counter1 

  	OCR1A   = 0x00;
  	OCR1C   = 0xFF;

	TIMSK  |= ( 1 << TOIE1 ); // enable Timer/Counter1 Overflow Interrupt
	TCCR1  |= _BV( CS13 ) | _BV( CS11 ) | _BV( CS10 ); // start
}

void SB_DELAY_Delay(uint8_t lDelay) {
	m_Timer1 = lDelay;

	while(m_Timer1 > 0x00);
}
