/*
 *
 * Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
 *
 */

#include <avr/io.h>

#include <stdint.h>

#include "include/sb_defs.h"
#include "include/sb_delay.h"

void SB_DEBUG_BlinkMIDIByte(volatile uint8_t *pByte) {
	uint8_t lMask = 0x01;

	while(1) {
		if(lMask > 0x00) { // bit 0 to 7 value display sequence
			if(*pByte & lMask) {
				PORTB |= _BV(SB_PIN_REC);
 			} else {
				PORTB |= _BV(SB_PIN_STOP);
			}
			
            lMask = lMask << 1;
			SB_DELAY_Delay(SB_DEBUG_DELAY_BIT_STEP);
        } else { // stop display sequence start
			PORTB |= _BV(SB_PIN_PLAY); // turn on PLAY
			SB_DELAY_Delay(SB_DEBUG_DELAY_STOP_LIGHT);
		}

        // turn off bit value LEDs
		PORTB &= ~_BV(SB_PIN_REC);
		PORTB &= ~_BV(SB_PIN_STOP);

		if(PORTB & _BV(SB_PIN_PLAY)) { // stop display sequence continuation
			PORTB &= ~_BV(SB_PIN_PLAY); // turn off PLAY
			SB_DELAY_Delay(SB_DEBUG_DELAY_STOP_SHADOW);

			PORTB |= _BV(SB_PIN_PLAY); // turn on PLAY
			SB_DELAY_Delay(SB_DEBUG_DELAY_STOP_LIGHT);
			
            PORTB &= ~_BV(SB_PIN_PLAY); // turn off PLAY
        
			SB_DELAY_Delay(SB_DEBUG_DELAY_EACH_STEP);	
			break;
		}

		SB_DELAY_Delay(SB_DEBUG_DELAY_EACH_STEP);
	}
}
