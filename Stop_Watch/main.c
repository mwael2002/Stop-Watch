/*
 * main.c
 *
 *  Created on: Sep 12, 2022
 *      Author: mwael
 */
#include <util/delay.h>
#include "STD_Types.h"
#include "DIO_interface.h"
#include "Timer_interface.h"
#include "Ex_Interrupt_interface.h"
#include "GIE_interface.h"
#include "seven_segment_interface.h"

Seven_Segment S1;
Seven_Segment S2;

U8 no_S1=0;
U8 no_S2=0;

U8 start_flag=0;
U8 stop_flag=0;

void App_init(void);

void time_increament(void);

void stop_restart_counter(void);
void start_counter(void);


void main(void){

App_init();

while(1){

}
}



void App_init(void){
	S1.Type=Anode;
	S1.enable_pin=DIO_Pin_7;
	S1.no_Port=Group_B;
	S1.enable_port=Group_D;

	S2.Type=Anode;
	S2.enable_pin=DIO_Pin_6;
	S2.no_Port=Group_A;
	S2.enable_port=Group_D;

	SS_init(&S1);
	SS_init(&S2);

	SS_ON(&S1);
	SS_ON(&S2);
	SS_Set_Number(&S1,0);
	SS_Set_Number(&S2,0);

	DIO_set_pin_direction(Group_D,DIO_Pin_2,INPUT);
	DIO_set_pin_direction(Group_D,DIO_Pin_3,INPUT);

	DIO_set_pin_value(Group_D,DIO_Pin_2,HIGH);
	DIO_set_pin_value(Group_D,DIO_Pin_3,HIGH);

	Ex_Int_call_back(start_counter,Ex_Int_0);
	Ex_Int_call_back(stop_restart_counter,Ex_Int_1);

	Ex_Int_set_level(Ex_Int_0,Ex_Int_Falling_edge);
	Ex_Int_set_level(Ex_Int_0,Ex_Int_Falling_edge);

	Ex_Int_Enable(Ex_Int_0);
	Ex_Int_Enable(Ex_Int_1);

	Global_Int_En_Dis(Global_Int_Enable);

}

void time_increament(void){

	if((no_S2==9)&&(no_S1==9)){
		no_S1=0;
		no_S2=0;
		SS_Set_Number(&S1,no_S1);
		SS_Set_Number(&S2,no_S2);
	    Overflow_restart_count(TIMER1_ID);
		return;
	}


	else if(no_S1==9){
		no_S1=0;
		no_S2++;
		SS_Set_Number(&S1,no_S1);
		SS_Set_Number(&S2,no_S2);
		Overflow_restart_count(TIMER1_ID);
	    return;
	}

	no_S1++;
	SS_Set_Number(&S1,no_S1);
	Overflow_restart_count(TIMER1_ID);



}

void start_counter(void){
	if(start_flag==0){
		_delay_ms(200);
		Timer_callback(TIMER1_ID,time_increament);
	    start_flag=1;
	    Timer1_init();
	}
	else if((stop_flag==1)&&(start_flag==1)){
		stop_flag=0;
		_delay_ms(200);
		Timer1_init();
	}
}

void stop_restart_counter(void){
	if(stop_flag==0){
        Timer_Int_Enable_Disable(TIMER1_OVERFLOW_INT,TIMER_INT_DISABLE);
		stop_flag=1;
		_delay_ms(200);
	}
	else if((stop_flag==1)&&(start_flag==1)){
        no_S1=0;
        no_S2=0;

		SS_Set_Number(&S1,0);
		SS_Set_Number(&S2,0);

		stop_flag=0;
		_delay_ms(200);

		Timer1_init();


	}

}
