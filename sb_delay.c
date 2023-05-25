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

	TCNT1 = 0x00;
}

void SB_DELAY_Delay(uint8_t lDelay) {
	m_Timer1 = lDelay;

	while(m_Timer1 > 0x00);
}
