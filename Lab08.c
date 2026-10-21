/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified: Brendan Novak
 *   ModDate: 10/20/2026
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 1200 // standard values from 1200 to 115200
#define DATABITS    8 // 5-8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=none, 1=odd, 2=even

/* Your code here.  */

void displayStartupMessage(void) {
	
	//NL
	uart_tx(CR); uart_tx(LF);
	uart_tx(CR); uart_tx(LF);
	
	const char line1[] = "RS232 Communications Server\r\n";
	const char line2[] = "Status: Active\r\n";
	const char line3[] = "Brendan Novak\r\n";
	const char line4[] = "EECE3624\r\n";
	const char line5[] = "Fall 2026\r\n";
	
	const char *strings[] = {line1, line2, line3, line4, line5};
	
	//startup message
	for (int i = 0; i < 5; i++) {
		const char *p = strings[i];
		while (*p != '\0') {
			uart_tx(*p);
			p++;
		}
	}
	
	//NL
	uart_tx(CR); uart_tx(LF);
	uart_tx(CR); uart_tx(LF);	
	
}

int main(void)
{
	unsigned char c;
	unsigned char end_count = 0;

	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	
	DDRA |= (1 << DDA0);
	PORTA &= ~(1 << PORTA0);
	
	displayStartupMessage();
	
	while(1)
	{
		/* your loop code here */
		c = uart_rx();
		
		//startup LED
		PORTA |= (1 << PORTA0);
		_delay_ms(75);
		PORTA &= ~(1 << PORTA0);
		
		//reset
		if (c == ESC) {
			displayStartupMessage();
			end_count = 0;
			continue;
		}
		
		if(c==CR){
			uart_tx(CR); uart_tx(LF);
		}
		else{
			uart_tx(c);
		}
		
		if (end_count == 0 && c == 'e') {
			end_count = 1;
		}
		else if (end_count == 1 && c == 'n') {
			end_count = 2;
		}
		else if (end_count == 2 && c == 'd') {
			end_count = 3;
		}
		else if (c == 'e') {
			end_count = 1;
		}
		else {
			end_count = 0;
		}
		
		
		if (end_count == 3) {
			
			uart_tx(CR); uart_tx(LF);
			uart_tx(CR); uart_tx(LF);
			
			const char byeMsg[] = "Bye";
			const char *p = byeMsg;
			while (*p != '\0') {
				uart_tx(*p);
				p++;
			}
			
			while (1){
				//hold
			}
		}
	}
	return 0;
}
