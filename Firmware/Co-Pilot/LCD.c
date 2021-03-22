/** @file LCD.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include LCD.h
 *  @bug no known bugs
 *	@warning the functions LCD_write_int and LCD_write_float have maximum digit number of 7 and has to be modified manually
 *  @see LCD.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "LCD.h"
 
 //------------DECLARE LOCAL VARIABLES------------//
 
/** @var uint8* D_PORT
 *  @brief holds memory address for the data port
 */

/** @var uint8* C_PORT
 *  @brief holds memory address for the command port
 */
 
 static volatile uint8 * D_PORT;
 static volatile uint8 * C_PORT;
 
 /** @brief initializes the LCD interface
  *  @param Data_PORT pointer to data port memory address.
  *  @param Control_PORT pointer to control port memory address.
  *  @return none.
  */
 void LCD_init(volatile uint8 * Data_PORT, volatile uint8 * Control_PORT){
   
   *Data_PORT = 0;			// ensure Data Port is cleared
   *Control_PORT = 0;		// ensure Control Port is cleared
   D_PORT = Data_PORT;		// store Data Port memory address in local storage
   C_PORT = Control_PORT;	// store Control Port memory address in local storage
	
   #ifdef EIGHT_BITS 
   
    //------------RESET THE LCD------------//
   
	*Control_PORT |= 0x01;
    *Data_PORT = 0x30;
	*Control_PORT &= ~0x01;
	_delay_ms(1);
	*Control_PORT |= 0x01;
	*Data_PORT = 0x30;
	*Control_PORT &= ~0x01;
	_delay_ms(1);
	*Control_PORT |= 0x01;
	*Data_PORT = 0x30;
	*Control_PORT &= ~0x01;
	_delay_ms(1);
	
	//------------CONFIGURE LCD BEHAVIOUR------------//
	
	LCD_cmd(0x38);	// 8-bit interface, 2-line mode, 5x11 dots format
	LCD_cmd(0x0C);	// display ON, cursor OFF, blink OFF
	LCD_cmd(0x06);	// cursor moves to the right, no display shift
	LCD_cmd(0x01);	// clear display

  #endif

  #ifdef FOUR_BITS
  
   //------------RESET THE LCD------------//
  
   *Control_PORT |= (0x01 << 5);
   *Data_PORT |= 0x03;
   *Control_PORT &= ~(0x01 << 5);
   _delay_ms(1);
   *Control_PORT |= (0x01 << 5);
   *Data_PORT |= 0x03;
   *Control_PORT &= ~(0x01 << 5);
   _delay_ms(1);
   *Control_PORT |= (0x01 << 5);
   *Data_PORT |= 0x03;
   *Control_PORT &= ~(0x01 << 5);
   *Data_PORT &= ~0x0F;
   _delay_ms(1);
   *Control_PORT |= (0x01 << 5);
   *Data_PORT |= 0x02;
   *Control_PORT &= ~(0x01 << 5);
   *Data_PORT &= ~0x0F;
   _delay_ms(1);
   
   //------------CONFIGURE LCD BEHAVIOUR------------//
   
   LCD_cmd(0x28);	// 4-bit interface, 2-line mode, 5x11 dots format
   LCD_cmd(0x0C);	// display ON, cursor OFF, blink OFF
   LCD_cmd(0x06);	// cursor moves to the right, no display shift
   LCD_cmd(0x01);	// clear display

  #endif
 }

/** @brief sends commands to the LCD
 *  @param cmd the 8-bit command to be sent.
 *  @return none.
 */
 void LCD_cmd(uint8 cmd){
    
  #ifdef EIGHT_BITS
	
	//------------SEND 8-BIT COMMAND------------//
	
	*C_PORT |= 0x01;	// enable LCD interface for new data (EN signal), Command register is selected
	_delay_us(600);
	*D_PORT = cmd;		// send entire 8-bit command
	_delay_us(600);
	*C_PORT &= ~0x01;	// clear control port

  #endif
	
  #ifdef FOUR_BITS
	
	//------------SEND HIGH NIBBLE------------//
	
	*C_PORT |= (0x01 << 5);				// enable LCD interface for new data (EN signal), Command register is selected
	_delay_us(800);
	*D_PORT |= ((cmd >> 4) & 0x0F);		// send high nibble of command
	_delay_us(800);
	*C_PORT &= ~(0x01 << 5);			// clear control port
	_delay_us(200);
	*D_PORT &= ~0x0F;					// clear data port

	//------------SEND LOW NIBBLE------------//

	*C_PORT |= (0x01 << 5);				// enable LCD interface for new data (EN signal), Command register is selected
	_delay_us(800);
	*D_PORT |= (cmd & 0x0F);			// send low nibble of command
	_delay_us(800);
	*C_PORT &= ~(0x01 << 5);			// clear control port
	_delay_us(200);
	*D_PORT &= ~0x0F;					// clear data port
	
   #endif
 }

/** @brief clears the text diplayed on LCD
 *  @return none.
 */
 void LCD_clear(void){

	LCD_cmd(0x01);	// clear display command

 }

/** @brief sets the position for the cursor
 *  @param row Y position of the cursor.
 *  @param col X position of the cursor.
 *  @return none.
 */
 void LCD_set_cursor(uint8 row, uint8 col){

	switch(row){

	case 0: LCD_cmd(0x80|col);	// first row
	break;
	case 1: LCD_cmd(0xC0|col);	// second row
	break;
	case 2: LCD_cmd(0x94|col);	// third row
	break;
	case 3: LCD_cmd(0xD4|col);	// fourth row
	break;
	default: LCD_cmd(0x80|col);	// default is first row
	break;
	
	}
  }

/** @brief displays a character on the LCD
 *  @param data the 8-bit character to be sent.
 *  @return none.
 */
 void LCD_write_chr(char data){
  
  //------------SEND 8-BIT DATA------------//
    
  #ifdef EIGHT_BITS
  
	*C_PORT |= 0x02;	// select Data register (RS signal)
	_delay_us(600);
	*C_PORT |= 0x03;	// enable LCD interface for new data (EN signal)
	_delay_us(600);
	*D_PORT = data;		// send entire 8-bit data
	_delay_us(600);
	*C_PORT &= ~0x03;	// clear control port

  #endif

  #ifdef FOUR_BITS
  
	//------------SEND HIGH NIBBLE------------//
  
	*C_PORT |= (0x02 << 5);				// select Data register (RS signal)
	_delay_us(800);
	*C_PORT |= (0x03 << 5);				// enable LCD interface for new data (EN signal)
	_delay_us(800);
	*D_PORT |= ((data >> 4) & 0x0F);	// send high nibble of data
	_delay_us(800);
	*C_PORT &= ~(0x03 << 5);			// clear control port	
	_delay_us(200);
	*D_PORT &= ~0x0F;					// clear data port
	
	//------------SEND LOW NIBBLE------------//
	
	*C_PORT |= (0x02 << 5);				// select Data register (RS signal)
	_delay_us(800);
	*C_PORT |= (0x03 << 5);				// enable LCD interface for new data (EN signal)
	_delay_us(800);
	*D_PORT |= (data & 0x0F);			// send low nibble of data
	_delay_us(800);
	*C_PORT &= ~(0x03 << 5);			// clear control port	
	_delay_us(200);
	*D_PORT &= ~0x0F;					// clear data port

	#endif
}

/** @brief displays a string on the LCD
 *  @param data pointer to the fist character of the string.
 *  @return none.
 */
 void LCD_write_string(const char * data){

   while(*data != '\0')

   LCD_write_chr(*data++);		// iterate through the array till the NULL terminator

 }
 
 /** @brief displays an integer number on the LCD
 *  @param data the integer number to be displayed (up to 16-bit value).
 *  @return none.
 */
 void LCD_write_int(uint16 data){
	 
	 char res[7];
	 itoa(data, res, 10);		// convert integer number to a string
	 LCD_write_string(res);		// send the converted string
	 
 }
 
 /** @brief displays a float number on the LCD
 *  @param data the float number to be displayed.
 *  @return none.
 */
 void LCD_write_float(float data){
	 
	 char res[7];
	 dtostrf(data, 6, 2,res);	// convert float number to a string
	 LCD_write_string(res);	    // send the converted string
	 
 }

 