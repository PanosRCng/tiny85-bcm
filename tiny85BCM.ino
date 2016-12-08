/*
 *
 *    a simple program for usage demonstration
 *
 *       sets pins PB3 and PB4 as bcm pins
 *	 and lights them up
 *
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include "tiny85_bcm.h"



int main(void)
{
	int bcm_pins[] = {PB3, PB4};

	bcm_init(bcm_pins, 2);

	// set enable interrupts
	sei();

	unsigned char levels[] = {0xFF, 0x0A};
	bcm_set(levels);


	while(1)
	{
		bcm_morphing();
	}

	return 0;
}

