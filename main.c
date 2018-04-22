/*
 * main.c  !!!!!!!!!!!! UWAGA .... w tym projekcie trzeba w��czy� w Eclipse
 *
 *                     �adowanie pliku *.eep do pami�ci EEPROM przy wgrywaniu wsadu HEX
 *
 *  Created on: 2010-03-31
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>

#include "LCD/lcd44780.h"

// definicje dla preprocesora


// dioda LED
#define LED_PIN (1<<PB0)			// definicja pinu do kt�rego pod��czona jest dioda
#define LED_TOG PORTB ^= LED_PIN	// makrodefinicja � zmiana stanu diody

// klawisz K1
#define KEY_PIN (1<<PB1)


// poni�sz� linijk� czasami trzeba wpisa� w eclipse przed definicjami
// zmiennych w pami�ci EEPROM, �eby nie podkre�la� sk�adni jako b��dnej
#define EEMEM __attribute__((section(".eeprom")))

const char PROGMEM tab1[] = {"FLASH"};
char EEMEM tab2[] = {"EEPROM"};

uint8_t znak_L[] = {16,16,18,20,24,16,31,0};		// wz�r znaku litery � w pami�ci RAM
uint8_t znak_o[] EEMEM = {4,32,14,17,17,17,14,0};	// wz�r znaku litery � w pami�ci EEPROM
const uint8_t znak_buzka[] PROGMEM = {14,17,27,17,17,21,17,14};	// wz�r znaku bu�ki w pami�ci FLASH
const uint8_t znak_termo[] PROGMEM = {4,10,10,10,17,31,31,14};	// wz�r znaku termometru w pami�ci FLASH

volatile uint8_t pwm1, pwm2, pwm3;

int main(void)
{
	DDRA |= (1<<PA7);	// ustawiamy kierunek linii pod�wietlenia LCD jako WYJ�CIE
	PORTA |= (1<<PA7);	// za��czamy pod�wietlenie LCD - stan wysoki

	// ****** inicjalizacja *********
		DDRB |= LED_PIN;		// kierunek pinu PB0 � wyj�ciowy
		DDRB &= ~KEY_PIN;		// kierunek piny PB1 - wej�ciowy
		PORTB |= KEY_PIN;		// podci�gamy pin klawisza do VCC

		// ****** p�tla g��wna programu  *********
	lcd_init();

	lcd_str_P(tab1);				// napis z pami�ci FLASH
	lcd_locate(0,10);
	lcd_str_P( PSTR("Linia1") );	// napis z pami�ci FLASH
	lcd_locate(1,0);
	lcd_str_E(tab2);				// napis z pami�ci EEPROM
	lcd_locate(1,10);
	lcd_str("Linia2");				// napis z pami�ci RAM

	// za�adowanie znak�w do pami�ci CGRAM
	lcd_defchar(0x80, znak_L);
	lcd_defchar_E(0x81, znak_o);
	lcd_defchar_P(0x82, znak_buzka);
	lcd_defchar_P(0x83, znak_termo);


	// wy�wietlenie w�asnych znak�w na LCD
	lcd_locate(0,7);
	lcd_str("\x80\x81");
	lcd_locate(1,8);
	lcd_str("\x82");
	lcd_locate(1,7);
	lcd_str("\x83");

	while(1);
	{
			if( !(PINC & KEY_PIN) )
			{
				_delay_ms(80);
				if( !(PINC & KEY_PIN) )
				{
					LED_TOG;
				}
			}
		}
}

