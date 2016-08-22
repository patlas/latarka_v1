/*
 * adc.c
 *
 * Created: 23.07.2016 15:07:16
 *  Author: PatLas
 */ 

 #include <avr/io.h>


void adc_init(void)
{
	ADMUX |= _BV(REFS1); //internal adc ref
	ADCSRA |= _BV(ADEN); // enable adc
	ADCSRA |= _BV(ADIE); // enable interrupt
	//ADCSRA |= _BV(ADATE); //enable trigger source -> ADCSRB -> free running mode
	ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); //prescaler set to FOSC/128
	
}

void adc_start(void)
{
	ADCSRA |= _BV(ADSC);
}


void adc_set_channel(uint8_t channel) // 0 - 3
{
	uint8_t mux = ADMUX;
	mux &= ~(_BV(MUX1) | _BV(MUX0));
	mux |= channel;
	ADMUX = mux;
}