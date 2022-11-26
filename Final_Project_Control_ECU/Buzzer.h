/*
 * Buzzer.h
 *
 *  Created on: Oct 31, 2022
 *      Author: abdoa
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"

#define BUZZER_PORT_ID	PORTC_ID
#define BUZZER_PIN_ID	PIN5_ID


void Buzzer_init();
void Buzzer_on();
void Buzzer_off();

#endif /* BUZZER_H_ */
