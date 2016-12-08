/*
 *    tiny85_bcm
 *
 *
 *
 *	- ATtiny85 Binary Code Modulation library
 *
 *
 *	
 *	- implements an 8bit BCM (256 possible levels) 
 *	  with ~490Hz BCM frequency (~2ms period), and morhping step 2
 *	  this means that the time it takes to go from level 0 to level 256 is ~1 second
 *
 *	  to modify use this formulas:
 *
 *	       F_TIMER = F_BCM * ( 2^bcm_bits -1 )
 *
 *	       (!) F_TIMER is the clock frequency after a possible prescalling 
 *
 *
 *
 *	- after included, 
 *		- call bcm_init() and then enable interrupts
 *		- in the main loop call bcm_morphing()
 *
 *
 *  	- developed (and tested) for ATtiny85 running 
 *	  at 8 MHz from its internal oscillator
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include "tiny85_bcm.h"



#define MORPHING_STEP 2

volatile int cycles = 0;
volatile unsigned char bitmask = 0x01;

unsigned char levels[6];
unsigned char new_levels[6];
unsigned char set_levels[6];
int *pins;
int n_pins;

volatile int stable = 0;



void bcm_init(int *bcm_pins, int num_of_pins)
{
	pins = bcm_pins;
	n_pins = num_of_pins;

	int i;
	for(i=0; i<n_pins; i++)
	{
		levels[i] = 0x00;
		new_levels[i] = 0x00;
		set_levels[i] = 0x00;

		DDRB |= (1 << pins[i]);
	}

	initCCR0AInterrupt();
}


void bcm_morphing(void)
{
	if(cycles == MORPHING_STEP)
    	{
      		cycles = 0;
      		morphing();

		if( stable )
		{
			int i;
			for(i=0; i<n_pins; i++)
			{
				new_levels[i] = set_levels[i];
			}

			stable = 0;	
		}
    	}
}


void bcm_set(unsigned char *n_levels)
{
	int i;
	for(i=0; i<n_pins; i++)
	{
		set_levels[i] = n_levels[i];
	}
}


void morphing(void)
{
	int i;
	for(i=0; i<n_pins; i++)
	{
		int diff_level = new_levels[i] - levels[i];

		if( diff_level != 0)
		{
			levels[i] += (unsigned char) ( diff_level / c_abs(diff_level) );
		}
		else
		{
			stable = 1;
		}
	}
}


void initCCR0AInterrupt()
{
	OCR0A  = 0xFF;    // number to count up to (0xFF = 255) (!) just for the first time
	TCCR0A = 0x02;    // clear Timer on Compare Match (CTC) mode
  	TCCR0B = 0x03;    // clock source CLK/64
	TIFR = 0x10;    // clear interrupt flag
	TIMSK = 0x10;   // TC0 compare match A interrupt enable	
}


int c_abs(int x)
{
	return ( (x < 0) ? -x : x );
}



// interrupt service routine (ISR) for Timer/Counter0 Compare Match A
ISR(TIMER0_COMPA_vect)
{
	OCR0A = bitmask;

	char port_data = 0;
    
	int i;
	for(i=0; i<n_pins; i++)
	{
		if( levels[i] & bitmask )
		{
			port_data |= (1 << pins[i]);
		}
	}

	PORTB = port_data;

	bitmask = (bitmask << 1);

	if(bitmask == 0x00)
	{
		bitmask = 0x01;
		cycles++;
	}
}

