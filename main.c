/*
 * LM92_LCD.c
 *
 * Created: 1/1/2016 5:19:35 PM
 * Author : David Schwehr
 * github: dssquared
 *
 * for use with an LM92 temperature sensor and and LCD w/ 44780 controller
 *  LM92 using I2C, LM92,I2C, and LCD libraries needed. pinDefines.h for atmage328p
 *  use pin defines for other micros or use atmel studio and create project w/ proper MCU
 *
 * Reads temperature from LM92 and calculates temperature in degrees Celsius and Fahrenheit
 *   then displays calculated temp on LCD. May eventually incorporate baro pressure and
 *   other variables to create a wireless weather station over Zigbee network.
 *
 *                 ***  not complete: a work in progress ***
 *             any suggestions contribute to my github:  dssquared
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


//  ---- function prototypes  ----  //
double getCtemp(void);
double getFtemp(void);
void displayTemp(byte temp, char unit);


int main(void){
	
	byte integerTemp;                // integer to be passed to displayTemp()
	double calculatedTemp;           // double to store return of get(x)Temp()
	
	//  ----  inits  ----  //
	initI2C();
	initLCD();
	LCD_string("LM92 Thermometer...");
	_delay_ms(3000);
    
    while (1) 
    {
		
		// display Fahrenheit
		calculatedTemp = getFtemp();
		integerTemp = (byte)calculatedTemp;  // only used for int display
		displayTemp(integerTemp, 'F');
		
		_delay_ms(3000);
		
		// display Celsius
		calculatedTemp = getCtemp();
		integerTemp = (byte)calculatedTemp;  // only used for int display
		displayTemp(integerTemp, 'C');
		
		_delay_ms(3000);
		LCD_clear();
		
    }  // end event loop
	
}  // end main()


/**
@brief    read LM92 temperature register and
            and calculate temp in degrees C
@param    none
@return   double degrees Celsius
*/
double getCtemp(){
	byte tempHighByte, tempLowByte;
	uint16_t tempWord;
	double cTemp;
	
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
	/**LCD_hex(tempWord>>3);
	_delay_ms(3000);*/
	
	// calculate temp
	cTemp = (tempWord >> 3) * 0.0625;
	
	return cTemp;
	
}  // end getCtemp()


/**
@brief    calls getCtemp() and calculates temperature
            in degress F from degrees C
@param    none
@return   double degrees Fahrenheit
*/
double getFtemp(){
	double fTemp;
	
	//fTemp = ((cTemp * 9.00) / 5.00) + 32.00;
	fTemp = (getCtemp() * 1.80) + 32.00;
	
	return fTemp;
	
}  // end getFtemp()


/**
@brief    displays 8-bit byte on LCD at current cursor position
             *** only an integer for now need to get float working ***
@param    8-bit byte of integer to be displayed
@return   none
*/
void displayTemp(byte temp, char unit){
	LCD_clear();
	LCD_string("Temp: ");
	LCD_integer(temp);
	LCD_char(DEG);
	LCD_char(unit);
}  // end displayTemp


