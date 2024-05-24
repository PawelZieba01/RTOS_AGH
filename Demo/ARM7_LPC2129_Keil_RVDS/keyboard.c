#include "keyboard.h"
#include <LPC21xx.H>

#define BUTTON0_bm (1<<4)
#define BUTTON1_bm (1<<6)
#define BUTTON2_bm (1<<5)
#define BUTTON3_bm (1<<7)



enum KeyboardState eKeyboardRead(void)
{
	unsigned char ucKeyboardBits = IO0PIN & 0xF0; 
	
		if((ucKeyboardBits & BUTTON0_bm) == 0) 
		{
			return BUTTON_0;
		}
		else if((ucKeyboardBits & BUTTON1_bm) == 0)
		{
			return BUTTON_1;
		}	 
		else if((ucKeyboardBits & BUTTON2_bm) == 0) 
		{
			return BUTTON_2;
		}
		else if((ucKeyboardBits & BUTTON3_bm) == 0) 
		{
			return BUTTON_3;
		}
		else 
		{
		return RELASED;
		}
}


void KeyboardInit(void)
{
	IO1DIR = IO1DIR & ~(BUTTON0_bm | BUTTON1_bm | BUTTON2_bm | BUTTON3_bm);
}
