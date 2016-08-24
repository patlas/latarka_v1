/*
 * AVR.c
 *
 * Created: 23.07.2016 15:01:58
 * Author : PatLas
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 16000000
#include <util/delay.h>

#include "adc.h"
#include "pwm.h"



#define ADC_COMP_VAL (uint16_t) 0
#define STEP 10 //pwm step

volatile uint8_t adc_channel; //??

typedef struct adc_t {
	uint8_t channel;
	uint8_t *OCR;
}adc_t;

adc_t adc[2];


volatile uint8_t index = 0;
ISR(ADC_vect)
{
	if(ADC < ADC_COMP_VAL)
	{
		*(adc[index].OCR) -= STEP;
	}
	else if(ADC > ADC_COMP_VAL)
	{
		*(adc[index].OCR) += STEP;
	}
	
	if(++index > 1)
		index = 0;

	adc_set_channel(adc[index].channel); // wait to correct channel change -> no free running mode -> manual trigger in interrupt!!!
	adc_start();
}

int main(void)
{
	//sei();

	adc[0].channel = 1;
	adc[0].OCR = &OCR1A;
	adc[1].channel = 3;
	adc[1].OCR = &OCR1B;

	adc_channel = 0;

	pwm_init();
	//adc_init();
	//adc_start();
	DDRB |=  1<<1 | 1<<0;
	PORTB &= ~(1<<0);

	OCR1A = 110;
	uint16_t x = 0;//ADC;

    while (1) 
    {
		//OCR1A = x;
		*(adc[0].OCR) = x;
		x+= 10;
		if(x>120) x=0;
		_delay_ms(100);
    }
}

