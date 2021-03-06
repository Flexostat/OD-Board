/*
 * pwm.c
 *
 * Created: 8/2/2011 3:10:29 PM
 *  Author: soslab
 */ 
#include "pwm.h"
#include "EEMAP.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

volatile uint16_t millist;
volatile uint16_t ust;
volatile uint16_t us_p_int;

//setup stir motor PWM
//TODO: convert this to work with timer2
void pwm_init() {
	/******STIR INIT*******/

	DDRD |= 0x08; //PD3:OSC2B output
	DDRD |= 0x04; //PD2 -> !MRST
	DDRD |= 0x40; //PD6 -> !MEN
	DDRD |= 0x80; //PD7 -> MDIR
	DDRB |= 0x01; //PB0 -> !MSLP
	DDRC |= 0x07; //PC0-PC2 output  -> MS1-MS3
	/************************************************************************/
	/*
	MS1 	MS2 	MS3 	Microstep Resolution
	Low 	Low 	Low 	Full step
	High	Low 	Low 	Half step
	Low 	High	Low 	Quarter step
	High	High	Low 	Eighth step
	High	High	High	Sixteenth step                                  */
	/************************************************************************/
	uint8_t stepmode = eeprom_read_byte(EE_ADDR_STEP);
	stepmode = stepmode == 0xff ? 0 : stepmode &0x07;  //take default full steps, mask lower 3 bits
	PORTC &= ~0x07; //PC0-PC2 low = full step
	PORTC |= stepmode;
	//PORTC |= 0x01; //half step
	PORTD |= 0x04; //pull motor reset high
	PORTD &= ~0x40; //pull motor enable low
	PORTB |= 0x01; //pull motor sleep high
	
	
	//CTC mode, toggle on compare match  B
	TCCR2A = 0x02 | 0x10; 
	//cksel /256,
	TCCR2B = 0x06;
	
	//toggle on counter top/2  
	//OCR2A = COUNTER_TOP/2;
	uint8_t ctop = eeprom_read_byte(EE_ADDR_COUNT);
	ctop = (ctop>32 && ctop<=64) ? ctop : COUNTER_TOP;
	OCR2A = ctop;
	us_p_int = (COUNTER_TOP+1)*(256/8);
	OCR2B = 0;
	
	TIMSK2 = 1<<OCIE2A; 
	
}

//one epoch is 2/STIR_SPEED seconds
//takes roughly 120 cycles every US_PER_INT
ISR (TIMER2_COMPA_vect) { 
  ust += us_p_int;
  while (ust>1000) {
	  ust-=1000;
	  millist +=1;
  }
}

uint16_t millis() {
  return millist;
}
