/*
 * Control_ECU_App.c
 *
 *  Created on: Nov 3, 2022
 *      Author: abdoa
 */

#include "Buzzer.h"
#include "DcMotor.h"
#include "std_types.h"
#include "Timer1.h"
#include "UART.h"
#include "avr/io.h"
#include "external_eeprom.h"
#include "util/delay.h"
#include "TWI.h"
#include "Timer0.h"


uint8 count =0;

typedef enum{
	failed_to_save,succeeded_to_save,wrong_password,right_password
}errors;

typedef enum {
	Check_this_password,save_new_pass,open,Buzzer_ON,Buzzer_OFF
}orders_from_HMI_ECU;

uint8 pass[5], repass[5],pass_in_memory[5]; // the same pass and repass in the HMI_ecu and pass_in_memory contains the pass in the EEPROM

/*
 * function to write pass in the EEPROM this pass entered by the user then sent by the UART
 * it first check that pass == repass the save it if yes
 * */
errors write_in_memory(uint8 pass[], uint8 repass[]){
	for (uint8 i=0; i<5; i++){
		if(pass[i]!=repass[i])
			return failed_to_save;
	}
	for(uint8 i =0;i<5;i++){
		EEPROM_writeByte(1+i, pass[i]); // 1+i is the address of the location in the eeprom that i'm doing to save in it the 5-byte pass
										// each byte is a number
		_delay_ms(10); // this delay is used between each two operations done on the eeprom
	}
	return succeeded_to_save;
}

/*
 * function to check that pass == pass_in_memory
 * */
errors verfiy_password_from_memory(uint8 pass[]){

	for (uint8 i = 0;i<5;i++){
		EEPROM_readByte(1+i, pass_in_memory+i);//saving the pass in eeprom in the arr
											  //pass_in_memory is the arr name and +i to increment the ptr as the function takes a ptr
		_delay_ms(10); // this delay is used between each two operations done on the eeprom
	}
	for(uint8 i =0;i<5;i++){
		if(pass[i]!=pass_in_memory[i])
			return wrong_password;
	}
	return right_password;
}

void count_3sec(){
	count++;
	if (count == 3){
		Timer1_deInit();
		DcMotor_Rotate(Anti_Clockwise, 100);
	}

}
void count_15sec(){
	count++;
	if(count==15){
		Timer1_deInit();
		DcMotor_Rotate(Stop, 100);
	}
}
void delay_timer1(uint8 sec){

	count =0;
	Timer1_ConfigType config_timer1={0,31250,FCPU_256,CTC};
	Timer1_init(&config_timer1);

	switch (sec){
	case 3 :
		Timer1_setCallBack(count_3sec);
		break;
	case 15:
		Timer1_setCallBack(count_15sec);
		break;
	}

}

int main(){

	TWI_ConfigType config_twi ={0b00000010,400000}; // this is address 1 since address is represented in 7 bits and lsb is for r/w
	TWI_init( & config_twi); // config of twi (i2c) to communicate with eeprom
	UART_config config_uart ={eight,disabled,one_bit,9600};
	UART_init(& config_uart);
	PWM_Timer0_init(); //used to generate pwm for the motor
	DcMotor_Init();
	Buzzer_init();
	SREG|=1<<7;

	errors status;
	uint8 choice;
	do{ // this bulk is to create pass at the beginning of the system
		for (uint8 i=0; i<5;i++)
			pass[i]=UART_receive_byte();
		for (uint8 i=0; i<5;i++)
			repass[i]=UART_receive_byte();
		status = write_in_memory(pass, repass);
		UART_send_byte(status);
	}while(status == failed_to_save);

	while(1){

		choice = UART_receive_byte();//waiting for orders from HMI_ecu
		switch(choice){
		case Check_this_password:
			for(uint8 i=0;i<5;i++){
				pass[i]= UART_receive_byte();//take the pass in open door or change pass
			}
			status = verfiy_password_from_memory(pass);
			UART_send_byte(status);
			break;
		case save_new_pass: //when choose to change pass then begin to send two passwords. the same bulk at the beginning of the main
			do{
				for (uint8 i=0; i<5;i++)
					pass[i]=UART_receive_byte();
				for (uint8 i=0; i<5;i++)
					repass[i]=UART_receive_byte();
				status = write_in_memory(pass, repass);
				UART_send_byte(status);
			}while(status == failed_to_save);
			break;
		case open:
			DcMotor_Rotate(Clockwise, 100);
			delay_timer1(15);
			while(count!=15){}
			delay_timer1(3);
			while(count!=3){}
			delay_timer1(15);
			while(count!=15){}
			break;
		case Buzzer_ON:
			Buzzer_on();
			break;
		case Buzzer_OFF:
			Buzzer_off();
			break;
		}
	}



}
