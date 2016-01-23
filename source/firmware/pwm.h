/*
 * pwm.h
 *
 * Created: 8/2/2011 3:10:41 PM
 *  Author: soslab
 */ 
#ifndef PWM_H_
#define PWM_H_
#include <avr/io.h>

//extern uint8_t tmr2ofcnt;
uint16_t millis();
uint16_t epoch();
void pwm_init();

/////STIR/////
//NOTE: changing STIRS_SPEED so it is not a factor of 2000
// will break millis().  ie 2000L/STIR_SPEED should have no remainder.

//pulses/sec = 8,000,000/256/2/COUNTER_TOP
//RPM when in half step mode = (puleses/sec)/96*60
//=> rpm = 9765.625/COUNTER_TOP
#define COUNTER_TOP 32
//interrupts second is:
// 8,000,000/256/COUNTER_TOP

#define US_PER_INT 1*(COUNTER_TOP+1)*(256/8)

#endif /* PWM_H_ */
