/** @file ADC.h
 *  @brief This driver supports interfacing ADC modules of 8-bit AVR MCU;
 *	this file includes function prototypes for ADC.c
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include delay.h and math.h in micro_config.h
 *  @bug no known bugs
 *  @see micro_config.h
 */


#ifndef ADC_H_
#define ADC_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "micro_config.h"

//------------DEFINE ADC PARAETERS ABBREVIATIONS------------//

#define AREF 0
#define AVCC 1
#define INT_REF 3
#define RIGHT_ADJ 0
#define LEFT_ADJ 1
#define ADC0 0
#define ADC1 1
#define ADC2 2
#define ADC3 3
#define ADC4 4
#define ADC5 5
#define ADC6 6
#define ADC7 7

//------------FUNCTION PROTOTYPES------------//

void ADC_Init(uint8 Vref, uint8 Left_Adj, uint8 Prescaler);
uint16 ADC_Read(uint8 Channel);


#endif /* ADC_H_ */