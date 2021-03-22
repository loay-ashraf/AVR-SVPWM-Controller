/** @file ADC.c
 *  @brief includes function definitions
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include ADC.h
 *  @bug no known bugs
 *  @see ADC.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "ADC.h"

/** @brief initializes the ADC in single ended conversion mode
 *  @param Vref the reference voltage (AREF, AVCC or internal).
 *  @param Left_Adj result is left adjusted or right adjusted (0 for right, 1 for left).
 *  @param Prescaler prescaler value for the ADC.
 *  @return none.
 */

 void ADC_Init(uint8 Vref, uint8 Left_Adj, uint8 Prescaler){

	uint8 P_Bits = (int) (log(Prescaler)/log(2));	// calculate prescaler bits
	ADMUX |= (Vref<<6);								// set the reference voltage bits
	ADMUX |= (Left_Adj<<5);							// set data adjustment bits
	ADCSRA |= P_Bits;								// set prescaler bits
	ADCSRA |= (1<<ADEN);							// enable the ADC

 }
/** @brief triggers instant conversion for the selected channel
 *  @param Channel the selected channel for the conversion.
 *  @return the conversion result (8-bit or 10-bit value).
 */

 uint16 ADC_Read(uint8 Channel){
    
	ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2));		// clear channel multiplexer bits
	if (Channel < 8)
	ADMUX |= Channel;									// set channel bits
	ADCSRA |= (1<<ADSC);								// trigger ADC conversion

	while(!(ADCSRA & (1<<ADIF)));						// wait for the conversion to finish

	ADCSRA |= (1<<ADIF);								// clear "conversion finished" flag

	_delay_ms(1);
	return ADC;											// return the value in the ADC Data register

 }