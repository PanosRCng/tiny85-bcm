#ifndef TINY85_BCM_H_
#define TINY85_BCM_H_


// initialize BCM
// inits internal arrays, sets the given pins as outputs, sets Timer/Counter0 interrupt
// (!) interrupts must be globally disabled
void bcm_init(int *bcm_pins, int num_of_pins);


// this method runs in the loop of the main program
// calls morphing and sets new bcm levels if exist
void bcm_morphing(void);


// sets the given levels as new bcm levels
void bcm_set(unsigned char *n_levels);


// the morphing mechanism
void morphing(void);


// init a Timer/Counter0 Compare Match Interrupt on OCR0A
void initCCR0AInterrupt(void);


// just a custom implementation of the abs() function
int c_abs(int x);


#endif
