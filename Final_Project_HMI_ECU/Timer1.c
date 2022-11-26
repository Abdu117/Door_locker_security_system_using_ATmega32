/*
 * Timer1.c
 *
 *  Created on: Nov 1, 2022
 *      Author: abdoa
 */
#include "Timer1.h"
#include "avr/io.h"
#include <avr/interrupt.h>

static volatile void(*function_ptr)(void) = NULL_PTR;

ISR(TIMER1_COMPA_vect){
	if(function_ptr!=NULL_PTR)
		{
			function_ptr();
		}
}

ISR(TIMER1_OVF_vect){
	if(function_ptr!=NULL_PTR)
			{
				function_ptr();
			}
}


void Timer1_init(const Timer1_ConfigType * config){
	TCNT1 = config->initial_value;
	OCR1A = config->compare_value;
	if(config->mode == Normal){
		TIMSK = 1 << TOIE1;
	}
	else
		TIMSK = 1 << OCIE1A;
	TCCR1A= (1<<FOC1A) | (1<<FOC1B) | ((TCCR1A & 0XFC )| (config->mode & 0x3));
	TCCR1B = ((TCCR1B & 0XE7)|((config->mode & 0xC)<<1)) | (config->prescaler);

}

void Timer1_deInit(){
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	OCR1A=0;
	TIMSK=0;
}

void Timer1_setCallBack(void(*a_ptr)(void)){

  function_ptr=a_ptr;
}
