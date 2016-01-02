/*
 * LM92_LCD.c
 *
 * Created: 1/1/2016 5:19:35 PM
 * Author : David
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "i2c.h"
#include "lm92.h"
#include "lcd.h"

//  ----  Defines/macros  ----  //
#define DEG 0XDF                  // LCD char for degrees symbol
typedef uint8_t byte;


int main(void){
	
	byte tempHighByte, tempLowByte, displayTemp;
	uint16_t tempWord;
	double cTemp, fTemp;
	
	//  ----  inits  ----  //
	initI2C();
	LCD_init();
	//LCD_string("testing");
	//LCD_char(DEG);
	_delay_ms(3000);
    
    while (1) 
    {
		// read temperature register from LM92
		i2cStart();
		i2cSend(LM92_ADDRESS_W);
		i2cSend(LM92_TEMP_REGISTER);
		i2cStart();
		i2cSend(LM92_ADDRESS_R);
		tempHighByte = i2cReadAck();
		tempLowByte = i2cReadNoAck();
		i2cStop();
		
		// assemble 2 8-bit readings to 16-bit word
		tempWord = 0x00;
		tempWord = (uint16_t)tempHighByte << 8;
		tempWord |= tempLowByte;
		
		// output LM92 data for debug
		LCD_hex(tempWord>>3);
		_delay_ms(3000);
		
		// calculate temp
		cTemp = (tempWord >> 3) * 0.0625;
		//fTemp = ((cTemp * 9.00) / 5.00) + 32.00;
		fTemp = (cTemp * 1.80) + 32.00;
		
		// display Fahrenheit
		displayTemp = (byte)fTemp;
		LCD_clear();
		LCD_string("Temp: ");
		LCD_integer(displayTemp);
		LCD_char(DEG);
		LCD_char('F');
		
		// display Celsius
		displayTemp = (byte)cTemp;
		LCD_line(1);
		LCD_string("Temp: ");
		LCD_integer(displayTemp);
		LCD_char(DEG);
		LCD_char('C');
		
		_delay_ms(5000);
		LCD_clear();
		
    }
	
}  // end main()



