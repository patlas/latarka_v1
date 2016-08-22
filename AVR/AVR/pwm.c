/*
 * pwm.c
 *
 * Created: 24.07.2016 12:07:12
 *  Author: PatLas
 */ 

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>


void pwm_init(void)
{
	
	PLLCSR |= _BV(PLLE); // set PLL as clock source (divided by 4 => 16MHz core clock)
	//while(PLLCSR & _BV(PLOCK)); // wait until PLL is being locked to 64MHz
	_delay_ms(10);
	PLLCSR |= _BV(PCKE); //enable asynchronous mode 64MHz from PLL source

	TCCR1 |= _BV(PWM1A); //enable PWM channel A
	TCCR1 |= _BV(COM1A0) | _BV(COM1A1); //OC1x set on compare match
	TCCR1 |= _BV(CS10); // asynchronous PCK clock
	GTCCR |= _BV(PWM1B); // enable PWM channel B
	GTCCR |= _BV(COM1B0) | _BV(COM1B1); //OC1x set on compare match 

	OCR1C = 127; // value as count limit overflow -> 500kHz PWM mode
}

//OCR1A, OCR1B - > set to controll fullfilment

