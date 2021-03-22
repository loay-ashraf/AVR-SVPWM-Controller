/** @mainpage About
 *  @author Loay Ashraf <loay.ashraf.96@gmail.com>
 *  @version 1.0.0 
 *
 *  This program is implemented on ATmega16/32 running 
 *	at 16 MHZ CPU clock; it basically processes and shows 
 *	current inverter status like Voltage, Frequency 
 *  and the speed of the motor and sets the main controller 
 *	output by communicating via parallel bus using 8-bit 
 *	data line, one ACK line and one READY line; It also 
 *	interfaces 16x2 parallel interface character LCD 
 *	as an output. 
 *	
 *	@pinassignments	
 *	
 *	- PA0: Speed Potentiometer signal
 *	- PA1-3: Phases current signal
 *	- PB0-3: LCD 4-bit Data signal
 *	- PB5: LCD EN signal
 *	- PB6: LCD RS signal
 *	- PB7: Direction Data signal
 *	- PC0-7: Parallel bus 8-bit Data signal
 *	- PD0-1: Direction Switch signal
 *	- PD3: Current Overload ERROR signal
 *	- PD5: Parallel bus READY signal
 *	- PD6: Parallel bus ACK signal	
 *  
 *	@see main.c
 */


/** @file main.c
 *  @brief includes the main function
 *  
 *  the main function acquires data like set Voltage, Phases current,
 *	Direction, etc.; and it sends that data to the LCD and the main 
 *	controller over the parallel bus and sends an ERROR signal if any
 *	current overload happens for any of the 3 phases.
 *
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include ADC.h, LCD.h, math.h and set F_CPU in micro_config.h
 *  @bug no known bugs
 *  @see micro_config.h
 */

//------------INCLUDE DEVICE DRIVERS------------//

#include "ADC.h"
#include "LCD.h"

//------------DEFINE DATA CALCULATION PARAMETERS------------//

#define VF_SLOPE 0.13333
#define MAX_VOLT 12
#define MIN_VOLT 0.2
#define AMMETER_VOLT 0.35 
#define RESISTOR_VALUE 1
#define CURRENT_THRESHOLD 2

//------------DECLARE GLOBAL VARIABLES------------//

/** @var float Volt
 *  @brief Stores current set Volt value
 */

/** @var float prev_Volt
 *  @brief Stores previous set Volt value to check for change
 */

 /** @var float Freq
 *  @brief Stores current commutation frequency
 */
 
 /** @var float C_PHa
 *  @brief Stores current Phase A current value
 */
 
 /** @var float C_PHb
 *  @brief Stores current Phase B current value
 */
 
 /** @var float C_PHc
 *  @brief Stores current Phase C current value
 */
 
 /** @var uint8 ACK_Flag
 *  @brief Stores previous ACK value to check for change
 */
float Volt, prev_Volt, Freq, C_PHa, C_PHb, C_PHc; 
uint8 ACK_Flag;

/** @brief the main function
 *
 *  see main.c description
 *
 *  @return exit status (1 or 0)
 */
int main(void){
    
	//------------INITIALIZE I/O PORTS------------//
	
	DDRA = 0x00;	// ADC PORT	
	DDRB = 0xFF;	// LCD DATA PORT
	DDRC = 0xFF;	// PARALLEL DATA PORT
	DDRD = 0x28;	// DIRECTION SW AND ACK/READY/FAULT PORT
	
	//------------INITIALIZE ADC AND LCD------------//
	
	ADC_Init(AVCC, RIGHT_ADJ, 128);	// AVCC as Vref, right sdjuted result (10-bit), 128 prescaler
	ADC_Read(ADC0);
	ADC_Read(ADC1);
	ADC_Read(ADC2);
	ADC_Read(ADC3);
	LCD_init(BO, BO);
	LCD_set_cursor(0,4);
	LCD_write_string("Stopped");

    while (1){
		
		//------------ACQUIRE CURRENT ACK STATUS AND DATA VIA ADC------------//
		
		ACK_Flag = PIND & 0x40;	// ACK SIGNAL (PD6)
		
		if(PIND&0x01){		// SWITCH TERMINAL #1 (PD0)
			PORTB |= 0x80;	// DIRECTION SIGNAL (PB7)
			Volt = ((ADC_Read(ADC0)/1023.0)*MAX_VOLT);
			if(Volt < MIN_VOLT)
				Volt = MIN_VOLT;
		}else if(PIND&0x02){	// SWITCH TERMINAL #2 (PD1)
			PORTB &= ~0x80;		// DIRECTION SIGNAL (PB7)
			Volt = ((ADC_Read(ADC0)/1023.0)*MAX_VOLT);
			if(Volt < MIN_VOLT)
				Volt = MIN_VOLT;
		}else{
			Volt = 0;}
			
		C_PHa = (((ADC_Read(ADC1)/1023.0)*AMMETER_VOLT)/RESISTOR_VALUE);
		C_PHb = (((ADC_Read(ADC2)/1023.0)*AMMETER_VOLT)/RESISTOR_VALUE);
		C_PHc = (((ADC_Read(ADC3)/1023.0)*AMMETER_VOLT)/RESISTOR_VALUE);
		
		//------------CHECK FOR CURRENT FAULT------------//
		
		if(C_PHa > CURRENT_THRESHOLD || C_PHb > CURRENT_THRESHOLD || C_PHc > CURRENT_THRESHOLD){
			PORTD |= 0x08;	// FAULT SIGNAL (PD3)
			while(1);		// wait for manual reset
		}
		
		//------------CHECK IF NEW DATA IS RECIEVED------------//
		
		if(Volt == prev_Volt || fabs(Volt-prev_Volt) < 0.1)
		continue;
		
		//------------OUTPUT DATA TO LCD------------//
		
		if(Volt == 0 && !(PIND&0x01) && !(PIND&0x02)){
			LCD_clear();
			LCD_set_cursor(0,4);
			LCD_write_string("Stopped");
		}else{
			LCD_set_cursor(0,0);
			LCD_write_string("Voltage:");
			LCD_set_cursor(1,0);
			LCD_write_string("Frequency: ");
			LCD_set_cursor(0,8);
			LCD_write_float(Volt);
			LCD_write_string(" V");
			LCD_set_cursor(1,11);
			LCD_write_int(90);
			LCD_write_string(" HZ");
		}
		
		//------------SET READY FLAG AND SEND DATA------------//
		
		PORTD |= 0x20;		// READY SIGNAL (PD5)
		PORTC = (Volt*10);	// DATA PORTC
		
		//------------WAIT FOR THE PILOT TO RECIEVE THE DATA------------//
		
		while (ACK_Flag == (PIND&0x40));
		PORTD &= ~0x20;		// READY SIGNAL (PD5)
		prev_Volt = Volt;	
    }
}

