/** @file SVPWM.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include SVPWM.h and adjust switching parameters
 *  @bug no known bugs
 *	@warning PWM frequency default is 7 KHZ, it can be reduced but can't be increased because of possible race conditions
 *  @see SVPWM.h
 */

//------------INCLUDE DRIVER HEADER FILE AND SINE LOOK-UP TABLE------------//

#include "SVPWM.h"
#include "tab_sin_233.h"

//------------DECLARE GLOBAL VARIABLES------------//
/** @var uint8 Frequency
 *  @brief current commutation frequency value
 */

/** @var uint8 theta
 *  @brief current angle value in the vector space
 */

/** @var uint8 theta_step
 *  @brief current angle step in the vector space
 */

/** @var uint8 sector
 *  @brief current sector number in the vector space
 */

/** @var uint16 PWM0
 *  @brief current compare register A value
 */

/** @var uint16 PWM1
 *  @brief current compare register B value
 */

/** @var uint16 PWM2
 *  @brief current compare register C value
 */

/** @var float Voltage
 *  @brief current output voltage value, can't exceed MAX_VOLTAGE value
 */

/** @var uint16 d1
 *  @brief vector period time variable 1
 */

/** @var uint16 d2
 *  @brief vector period time variable 2
 */
extern uint8 Frequency, theta, theta_step, sector;
extern uint16 PWM0, PWM1, PWM2;
extern float Voltage;
static uint16 d1, d2;

/** @brief initializes timer 1
  *  @return none.
  */
void timer_init(void){
	
	ICR1 = MAX_PWM;
	TCCR1A = 0xFC;	// no prescaler, phase and frequency correct mode
	TCCR1B = 0x11;	// set when up counting, clear when down counting
	
}
/** @brief loads timer 1 compare registers with new values
  *  @param C0 compare register A value.
  *  @param C1 compare register B value.
  *  @param C2 compare register C value.
  *  @return none.
  */
void timer_load(uint16 C0, uint16 C1, uint16 C2){
	
	OCR1A = C0;	// compare register A
	OCR1B = C1;	// compare register B
	OCR1C = C2;	// compare register C
	
}

/** @brief shuts down the timer and Phases output
  *	@return none.
  */
void shutdown(void){
	// timer 1 and its outputs are off
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0;
	ICR1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	OCR1C = 0;
	PORTB = 0xE0;	// all three phases are high
	
}

/** @brief calculates new compare register values
  *	@return none.
  */
void SVPWM(void){
	
	d1 = 47*Voltage*tab_sin[MAX_THETA-theta];
	d2 = 47*Voltage*tab_sin[theta];
	
	switch (sector){
		
	case 1: {PWM0 = MAX_PWM_DIV_TWO-d1-d2;
			 PWM1 = MAX_PWM_DIV_TWO+d1-d2;
			 PWM2 = MAX_PWM_DIV_TWO+d1+d2;}	
		break;
	
	case 2: {PWM0 = MAX_PWM_DIV_TWO-d1+d2;
			 PWM1 = MAX_PWM_DIV_TWO-d1-d2;
			 PWM2 = MAX_PWM_DIV_TWO+d1+d2;}
		break;
		
	case 3: {PWM0 = MAX_PWM_DIV_TWO+d1+d2;
			 PWM1 = MAX_PWM_DIV_TWO-d1-d2;
			 PWM2 = MAX_PWM_DIV_TWO+d1-d2;}
		break;
		
	case 4: {PWM0 = MAX_PWM_DIV_TWO+d1+d2;
		     PWM1 = MAX_PWM_DIV_TWO-d1+d2;
			 PWM2 = MAX_PWM_DIV_TWO-d1-d2;}	
		break;
		
	case 5: {PWM0 = MAX_PWM_DIV_TWO+d1-d2;
			 PWM1 = MAX_PWM_DIV_TWO+d1+d2;
			 PWM2 = MAX_PWM_DIV_TWO-d1-d2;}
		break;
		
	case 6: {PWM0 = MAX_PWM_DIV_TWO-d1-d2;
			 PWM1 = MAX_PWM_DIV_TWO+d1+d2;
			 PWM2 = MAX_PWM_DIV_TWO-d1+d2;}
		break;
		
	default: {PWM0 = MAX_PWM_DIV_TWO;
			  PWM1 = MAX_PWM_DIV_TWO;
			  PWM2 = MAX_PWM_DIV_TWO;}	
		break;	  				
	}
	
	theta += theta_step;	// increment vector angle
	
	if (theta > MAX_THETA)
	{
		
		theta = 0;
		if(++sector > 6)
			sector = 1;
	}
}

/** @brief shuts down the timer and Phases output
  *	@param Freq commutation frequency
  *	@return corresponding voltage value.
  */
uint8 VF_control(uint8 Freq){
	
	return Freq*VF_SLOPE;
}