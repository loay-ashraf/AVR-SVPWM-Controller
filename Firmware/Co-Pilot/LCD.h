/** @file LCD.h
 *  @brief This driver supports interfacing of parallel interface character
 *	LCD (both 16x2 and 20x4); this file includes function prototypes for LCD.c
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include delay.h and stdlib.h in micro_config.h
 *  @bug no known bugs
 *	@warning data mode has to be selected here
 *  @see micro_config.h
 */


#ifndef LCD_H_
#define LCD_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "micro_config.h"

//------------DEFINE PORT POINTERS ABBREVIATIONS AND DATA MODE------------//

#define AO &PORTA
#define BO &PORTB
#define CO &PORTC
#define DO &PORTD
//#define EIGHT_BITS 1
#define FOUR_BITS 

//------------FUNCTION PROTOTYPES------------//

void LCD_init(volatile uint8 * Data_PORT, volatile uint8 * Control_PORT);
void LCD_cmd(uint8 cmd);
void LCD_clear(void);
void LCD_set_cursor(uint8 row, uint8 col);
void LCD_write_chr(char data);
void LCD_write_string(const char * data);
void LCD_write_int(uint16 data);
void LCD_write_float(float data);
 
#endif /* LCD_H_ */