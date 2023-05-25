/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

#include <avr/io.h>

#include <stdint.h>

#include "include/sb_defs.h"
#include "include/sb_delay.h"

void SB_TASCAM_Record(void) {
	PORTB |= _BV(SB_PIN_REC); // press REC
	SB_DELAY_Delay(SB_TASCAM_DELAY_RECORD_PRE);

	PORTB |= _BV(SB_PIN_PLAY); // press PLAY
	SB_DELAY_Delay(SB_TASCAM_DELAY_RECORD_IN);
	PORTB &= ~_BV(SB_PIN_PLAY); // release PLAY

	SB_DELAY_Delay(SB_TASCAM_DELAY_RECORD_POST);
	PORTB &= ~_BV(SB_PIN_REC); // release REC
}

void SB_TASCAM_StopAndRewind(void) {
	PORTB |= _BV(SB_PIN_STOP); // press STOP
	SB_DELAY_Delay(SB_TASCAM_DELAY_REWIND_PRE);

	PORTB |= _BV(SB_PIN_REW); // press REWIND
	SB_DELAY_Delay(SB_TASCAM_DELAY_REWIND_IN);
	PORTB &= ~_BV(SB_PIN_REW); // release REWIND

	SB_DELAY_Delay(SB_TASCAM_DELAY_REWIND_POST);
	PORTB &= ~_BV(SB_PIN_STOP); // release STOP
}
