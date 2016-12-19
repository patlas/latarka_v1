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

/*	ADC Comp value #define ADC_COMP_VAL
	180 -> 736mA
	100 -> 440mA
	50  -> 230mA
	15  -> 83mA
*/

#define ADC_COMP_VAL (uint16_t) 180//0//512
#define STEP 1 //pwm step
#define PWM_MAX 127

volatile uint8_t adc_channel; //??

typedef struct adc_t {
	uint8_t channel;
	uint8_t *OCR;
	uint8_t pwm_val;
}adc_t;

volatile adc_t adc[2];


volatile uint8_t index = 0;
volatile uint8_t m_index[2] = {0};
volatile uint16_t adc_val[2] = {0};
//volatile uint8_t pwm_val = PWM_MAX/2;

ISR(ADC_vect)
{
	//uint16_t adc_val = ADCL;
	//adc_val |= ((ADCH<<8) & 0xF00);
	adc_val[index] += ADC;
	m_index[index]++;
	adc_val[index] /= 2;//m_index; 

	if(m_index[index] >= 10)
	{
		m_index[index] = 0; /// odwrotna logika!!!!
		if(adc_val[index] < ADC_COMP_VAL)
		{
			adc[index].pwm_val -= STEP;
			if(adc[index].pwm_val < STEP)
				adc[index].pwm_val = STEP;
			*(adc[index].OCR) = adc[index].pwm_val;
			//PORTB |= 1<<3;
		}
		else if(adc_val[index] > ADC_COMP_VAL)
		{
			adc[index].pwm_val += STEP;
			if(adc[index].pwm_val > PWM_MAX)
				adc[index].pwm_val = PWM_MAX;
			*(adc[index].OCR) = adc[index].pwm_val;
			//PORTB &= ~(1<<3);
		}
		//adc_val = 0;
	}
	//if(++index > 1)
	//	index = 0;
	index ^= 0x01;
	adc_set_channel(adc[index].channel); // wait to correct channel change -> no free running mode -> manual trigger in interrupt!!!
	adc_start();
}

int main(void)
{
	

	adc[0].channel = 1;
	adc[0].OCR = &OCR1A;
	adc[0].pwm_val = 0;
	adc[1].channel = 3;
	adc[1].OCR = &OCR1B;
	adc[1].pwm_val = 0;

	adc_channel = 0;

	DDRB |=  1<<1 | 1<<0 /*| 1<<3*/ | 1<<4;
	DDRB &= ~(1<<2 | 1<<3);
	//PORTB &= ~(1<<3);
	_delay_ms(500);

	pwm_init();
	adc_init();
	
	

	PORTB &= ~(1<<0);
	//PORTB |= (1<<3);


	//OCR1A = 110;
	uint16_t x = 0;//ADC;
	adc_set_channel(adc[index].channel);
	adc_start();
	sei();

    while (1) 
    {
		//OCR1A = x;
		/**(adc[0].OCR) = x;
		x+= 10;
		if(x>120) x=0;*/
		//PORTB ^= (1<<3);
		//_delay_ms(500);
    }
}

