/*
 * Timer1.h
 *
 *  Created on: Nov 1, 2022
 *      Author: abdoa
 */

#ifndef TIMER1_H_
#define TIMER1_H_
#include "std_types.h"

typedef enum{
	no_clk,FCPU_1,FCPU_8,FCPU_64,FCPU_256,FCPU_1024,external_clk_falling_edge,external_clk_rising_edge
}Timer1_Prescaler;

typedef enum{
	Normal,CTC=4
}Timer1_Mode;

typedef struct {
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer1_Prescaler prescaler;
Timer1_Mode mode;
} Timer1_ConfigType;

void Timer1_init(const Timer1_ConfigType * config);
void Timer1_deInit();
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
