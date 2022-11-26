/*
 * HMI_ECU_App.c
 *
 *  Created on: Nov 3, 2022
 *      Author: abdoa
 */
#include"keypad.h"
#include "LCD.h"
#include "std_types.h"
#include "Timer1.h"
#include "UART.h"
#include "util/delay.h"
#include "avr/io.h"

uint8 pass[5], repass[5],count=0; // pass to save the first password, repass to save the second one
								  // and count is used to count the clicks of timer1
typedef enum{
	failed_to_save,succeeded_to_save,wrong_password,right_password // commands sent by Control_ecu to give ack to HMI that the pass
}errors;														   // is saved etc...

typedef enum {
	Check_this_password,save_new_pass,open,Buzzer_ON,Buzzer_OFF  // commands sent by HMI_ecu to give orders to Control_ecu
}orders_from_HMI_ECU;

/*
 * function to get values from keypad and show '*' in the lcd
 * */
void Create_system_password(uint8 pass[], uint8 repass[]){
	LCD_clearScreen();
	_delay_ms(200);
	LCD_displayString("Plzz enter Pass: ");
	LCD_moveCursor(1,0);
	for(uint8 i =0 ; i<5 ;i++){
		do {
			pass[i]=KEYPAD_getPressedKey();
		}while(pass[i]>9); 									    // to validate that the password not +,-,=
		LCD_displayCharacter('*');
		//LCD_intgerToString(pass[i]);
		_delay_ms(300);											// delay for the push button during clicking on it

	}
	while(KEYPAD_getPressedKey()!=13);							//waiting for enter(ON/C)
	LCD_clearScreen();
	_delay_ms(200);												//extra delay
	LCD_displayString("Plz re-enter it: ");
	LCD_moveCursor(1, 0);
	for(uint8 i =0 ; i<5 ;i++){
		do {
			repass[i]=KEYPAD_getPressedKey();
		}while(repass[i]>9);
		LCD_displayCharacter('*');
		//LCD_intgerToString(repass[i]);
		_delay_ms(300);
	}
	while(KEYPAD_getPressedKey()!=13);
	LCD_clearScreen();
}

/*
 * function to send the two passwords to the Control_ecu by the UART
 * */
errors Check_created_password(uint8 pass[], uint8 repass[]){

	for(uint8 i =0;i<5;i++)
		UART_send_byte(pass[i]);
	for(uint8 i =0;i<5;i++)
		UART_send_byte(repass[i]);

	errors status = UART_receive_byte(); // waiting to know that the two passwords are matched and is saved
	return status;

}

/*
 *  function to verify the given password from the keypad with the password saved in the EEPROM
 * */
errors verify_password(uint8 pass[]){

	UART_send_byte(Check_this_password); //order the Control_ecu to check it with the password saved in the EEPROM
	for(uint8 i =0;i<5;i++)
		UART_send_byte(pass[i]);
	errors status = UART_receive_byte(); //waiting to confirm that the password is correct or not
	return status;
}

/*
 * function to count 18sec by timer1
 * */
void count_18sec(){
	count++;
	if(count==18){
		Timer1_deInit();
		LCD_clearScreen();
		LCD_displayString("Door is locking");
	}
}

/*
 * function to count 15sec by timer1
 * */
void count_15sec(){
	count++;
	if(count==15){
		Timer1_deInit();
		LCD_clearScreen();
	}
}

/*
 * function to count 60sec by timer1
 * */
void count_60sec(){
	count++;
	if(count==60){
		Timer1_deInit();
		LCD_clearScreen();
		UART_send_byte(Buzzer_OFF); // hear i controlled the timing of the buzzer by the HMI_ecu not the Control_ecu unlike the motor

	}
}

/*
 * function to setCallBack of timer1 with one of the functions above
 * */
void delay_timer1(uint8 sec){

	count =0;
	Timer1_ConfigType config_timer1={0,31250,FCPU_256,CTC}; //this config count one second
	Timer1_init(&config_timer1);

	switch (sec){
	case 15:
		Timer1_setCallBack(count_15sec);
		break;
	case 18:
		Timer1_setCallBack(count_18sec);
		break;
	case 60:
		Timer1_setCallBack(count_60sec);
		break;

	}

}

/*
 * function to print the main menu and return either + or -
 * */
uint8 Main_options(){
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass");
	return KEYPAD_getPressedKey();
}

/*
 * function to open the door
 * */
void Open_door(uint8 pass[]){
	uint8 false_pass_count=0;
	while(false_pass_count<3){ //to count false password max three times
		LCD_clearScreen();
		_delay_ms(200);
		LCD_displayString("plz enter Pass: ");
		LCD_moveCursor(1, 0);
		for(uint8 i =0 ; i<5 ;i++){
			do {
				pass[i]=KEYPAD_getPressedKey();
			}while(pass[i]>9);
			//LCD_intgerToString(pass[i]);
			LCD_displayCharacter('*');
			_delay_ms(300);
		}
		while(KEYPAD_getPressedKey()!=13);
		errors status = verify_password(pass); //waiting the Control_ecu to verify and give an ack or confirm

		if (status == right_password )
		{

			LCD_clearScreen();
			LCD_displayString("Door is Unlocking");
			UART_send_byte(open);//telling the Control_ecu to open which turn on the motor
			delay_timer1(18);
			while(count!=18){} // this polling to make the code stuck until finishing the 18 sec so that the screen can show only
			delay_timer1(15);  // door is unlocking
			while(count!=15){}
			break;
		}
		else
		{
			false_pass_count++; // increment it tell reaching 3
		}
	}
	if(false_pass_count==3){

		LCD_clearScreen();
		LCD_displayString("ERROR");
		UART_send_byte(Buzzer_ON);
		delay_timer1(60);
		while(count!=60){}
	}


}
void Change_password(uint8 pass[]){
	uint8 false_pass_count=0;
	while(false_pass_count<3){
		LCD_clearScreen();
		_delay_ms(200);
		LCD_displayString("plz enter Pass: ");
		LCD_moveCursor(1, 0);
		for(uint8 i =0 ; i<5 ;i++){
			do {
				pass[i]=KEYPAD_getPressedKey();
			}while(pass[i]>9);
			//LCD_intgerToString(pass[i]);
			LCD_displayCharacter('*');
			_delay_ms(300);

		}
		while(KEYPAD_getPressedKey()!=13);
		errors status = verify_password(pass);
		if (status == right_password )
		{
			do{									//the same code at the beginning of the main is used to get two passwords
				LCD_clearScreen();
				_delay_ms(200);
				Create_system_password(pass, repass);
				UART_send_byte(save_new_pass);
				status = Check_created_password(pass, repass);
				if (status == failed_to_save){
					LCD_displayString("Not Matched");
					_delay_ms(1500);
					LCD_clearScreen();
				}
			}while(status== failed_to_save);
			break;
		}
		else
		{
			false_pass_count++;
		}
	}
	if(false_pass_count==3){
		LCD_clearScreen();
		LCD_displayString("ERROR");
		UART_send_byte(Buzzer_ON);
		delay_timer1(60);
		while(count!=60){}
	}
}


int main(){

	SREG|=(1<<7); //used to enable Interrupts for timer1
	LCD_init();
	UART_config config={eight,disabled,one_bit,9600}; // config to transmit data with the second micro
	UART_init(&config);
	errors status;
	do{				// is used to create new pass by getting two passwords
		Create_system_password(pass, repass);
		status = Check_created_password(pass, repass);
		if (status == failed_to_save){
			LCD_displayString("Not Matched");
			_delay_ms(1500);
			LCD_clearScreen();
		}
	}while(status== failed_to_save);


	while(1){

		uint8 choice;
		choice = Main_options(); // always print the main menu when finish specific task
		switch (choice){
		case '+':
			Open_door(pass);
			break;

		case '-':
			Change_password(pass);
			break;
		}

	}


}


