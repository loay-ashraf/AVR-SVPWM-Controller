/** @file SVPWM.h
 *  @brief This library lays a foundation for
 *	outputting sinusoidal PWM using the Space
 *	Vector algorithm, and it can be ported to 
 *	nearly any device; this file includes 
 *	function prototypes for SVPWM.c
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include micro_config.h
 *  @bug no known bugs
 *  @see micro_config.h
 */ 


#ifndef SVPWM_H_
#define SVPWM_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "micro_config.h"

//----------------DEFINE SWITCHING PARAMETERS---------------//
//--------------SW_FREQUENCY = (16*10^6)/(2*MAX_PWM)--------------//

#define MAX_FREQUENCY 90
#define MAX_VOLTAGE 12
#define MAX_PWM 1140
#define MAX_PWM_DIV_TWO 570
#define MAX_THETA 232
#define VF_SLOPE 0.133
#define SW_FREQUENCY 7018L
#define FORWARD 1
#define BACKWARD 0

//------------FUNCTION PROTOTYPES------------//

void timer_init(void);
void timer_load(uint16 C0, uint16 C1, uint16 C2);
void shutdown(void);
void SVPWM(void);
uint8 VF_control(uint8 Freq);



#endif /* SVPWM_H_ */