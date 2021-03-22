/** @mainpage About
 *  @author Loay Ashraf <loay.ashraf.96@gmail.com>
 *  @version 1.0.0 
 *
 *  This program is implemented on ATmega128 running 
 *	at 16 MHZ CPU clock; it basically controls output 
 *	of three phase bridge inverter and receives set of
 *	switching parameters from the co-pilot like the 
 *	required voltage, commutation frequency, etc. and
 *	sets the output based on the data received; It
 *	communicates with the co-pilot via parallel bus 
 *	using 8-bit data line, one ACK line and one READY 
 *	line, also it receives direction signal via direct
 *	pin connection and can be interrupted directly by
 *	the co-pilot in case of current overload on any of
 *	the three phases. 
 *	
 *	@pinassignments	
 *	
 *	- PA7: Direction Data signal
 *	- PB5-7: Phases control signal
 *	- PC0-7: Parallel bus 8-bit Data signal
 *	- PD0: Current Overload ERROR signal
 *	- PD5: Parallel bus READY signal
 *	- PD6: Parallel bus ACK signal
 *  
 *	@see main.c
 */

/** @file main.c
 *  @brief includes the main function
 *  
 *  the main function keeps running the SVPWM algorithm
 *	during each iteration of the while loop, sets new
 *	compare register values for the 16-bit timer1 and 
 *	ONLY checks for new data from the co-pilot on the
 *	beginning of new commutation cycle.
 *
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include SVPWM.h, delay.h and set F_CPU in micro_config.h
 *  @bug no known bugs
 *  @see micro_config.h
 */

//------------INCLUDE SVPWM LIBRARY------------//

#include "SVPWM.h"

//------------DEFINE GLOBAL VARIABLES------------//

/** @var uint16 direction
 *  @brief direction flag (0 for backward, 1 for forward)
 */

uint8 Frequency, theta, theta_step, sector, direction;
uint16 PWM0, PWM1, PWM2;
float Voltage;

/** @brief the main function
 *
 *  see main.c description
 *
 *  @return exit status (1 or 0)
 */
int main(void){
   
   //------------INITIALIZE I/O PORTS------------//
   
   DDRA = 0x00;
   DDRB = 0xE0;
   DDRC = 0x00;
   DDRD = 0x40;
   
   //------------ALIGN ROTOR SHAFT------------//
   
   PORTB |= 0x20;
   _delay_ms(1);
   PORTB &= ~0x20;
   
   //------------WAIT TO RECIEVE DATA------------//
   
   while(!(PIND & 0x20));
   Voltage = (PINC/10.0);
   Frequency = 50;
   theta_step = ((long)360*233*Frequency)/(SW_FREQUENCY*60);	// calculate angle step based on commutation frequency
   direction = ((PINA & 0x80) >> 7);
   PORTD ^= 0x40;
   
   //------------RUN SVPWM CALCULATIONS AND INITIALIZE TIMER------------//
   
   sector = 1;
   theta = 0;
   SVPWM();
   if(direction == FORWARD)
   timer_load(PWM0, PWM1, PWM2);	// forward direction
   else
   timer_load(PWM0, PWM2, PWM1);	// backward direction
   timer_init();
   
	while (1){
		
		//------------CHECK FOR CURRENT FAULT SIGNAL------------//
		
		if(PIND & 0x01){
			shutdown();	
			while(1);	// wait for manual reset	
		}
		
		//------------CHECK FOR NEW DATA AT THE BEGINNING OF THE NEW CYCLE------------//
		
		if(sector == 1 && theta == 0 && PIND & 0x20){
			
			Voltage = (PINC/10.0);
			if(Voltage == 0){
				
				//------------SEND ACK AND SHUTDOWN------------//
				
				PORTD ^= 0x40;
				shutdown();
				
				//------------WAIT TO RECIEVE NEW DATA------------//
				
				while(!(PIND & 0x20));
				Voltage = (PINC/10.0);
				Frequency = 50;
				theta_step = ((long)360*233*Frequency)/(SW_FREQUENCY*60);	// calculate angle step based on commutation frequency
				direction = ((PINA & 0x80) >> 7);
				PORTD ^= 0x40;
				
				//------------REINTIALIZE------------//
				
				PORTB = 0x00;
				SVPWM();
				if(direction == FORWARD)
				timer_load(PWM0, PWM1, PWM2);	// forward direction
				else
				timer_load(PWM0, PWM2, PWM1);	// backward direction
				timer_init();
			}else{	
				Frequency = 50;
				theta_step = (int) 0.2*Frequency;	// calculate angle step based on commutation frequency
				direction = ((PINA & 0x80) >> 7);
				PORTD ^= 0x40;
			}	
		}
		
		//------------RUN SVPWM CALCULATIONS FOR EACH VECTOR AND UPDATE COMPARE REGISTERS------------//
		
		SVPWM();
		while(!(TIFR & (1<<ICF1)));
		if(direction == FORWARD)
		timer_load(PWM0, PWM1, PWM2);	// forward direction
		else
		timer_load(PWM0, PWM2, PWM1);	// backward direction
		TIFR |= (1<<ICF1);				// clear input capture flag

    }
}