/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* SPDX-License-Identifier: Apache-2.0                                                                     */
/* Copyright(c) 2020 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/


//***********************************************************************************************************
//  File Function: MS51 ADC simple convert demo code
//***********************************************************************************************************
#include "MS51_32K.h"
#define clk_high P1|=(0x01)<<0
#define clk_low P1&=~(0x01)<<0
#define strobe_high P1|=(0x01)<<1
#define strobe_low P1&=~(0x01)<<1
#define dio_high P1|=(1<<2)
#define dio_low P1&=~(1<<2)
bit  ADC_CONT_FINAL_FLAG; 
/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/

unsigned char shiftIn(void) {
  unsigned char value = 0;
  unsigned char i;
	P10_PUSHPULL_MODE;
	
  for (i = 0; i < 8; ++i) {
    clk_high;
    value |= ((P1>>2)&1)<< i;
    //printf("%02X\n",(value&0xFF));
		
    clk_low;
  }
  return value;
}

void shiftOut(unsigned char val){
	unsigned char i;
  for (i = 0; i < 8; i++){
    if(val & 1){
      dio_high;			
			//P35=1;
			//Timer2_Delay(24000000,128,300,100);
    }
    else{
      dio_low;
			//P35=0;
			//Timer2_Delay(24000000,128,300,100);
		}
    val >>= 1;
    clk_high;
    clk_low;    
  }
}

void sendCommand(unsigned char value)
{
	P12_PUSHPULL_MODE;
  strobe_low;
  shiftOut(value);
  strobe_high;
}
 
void reset()
{
	unsigned char i;
  sendCommand(0x40); // set auto increment mode
  strobe_low;
  shiftOut(0xc0);   // set starting address to 0
  for(i = 0; i < 16; i++)
  {
    shiftOut(0x00);
  }
  strobe_high;
}
unsigned char readButtons(void)
{
  unsigned char buttons = 0;
  unsigned char i;
	P12_PUSHPULL_MODE;
	
  strobe_low;
  shiftOut(0x42);
  P12_INPUT_MODE;
  for (i = 0; i < 4; i++)
  {
    unsigned char v = shiftIn() << i;
    buttons |= v;
  }
 
  P12_PUSHPULL_MODE;
	
  strobe_high;
  return buttons;
}

void main (void) 
{
/* Initial UART0 for printf */
	
    unsigned int temp;
	P35_PUSHPULL_MODE;
/* UART0 initial setting
  * include sys.c in Library for modify HIRC value to 24MHz
  * include uart.c in Library for UART initial setting
*/
    //MODIFY_HIRC(HIRC_24);
    P06_PUSHPULL_MODE;
    UART_Open(24000000,UART0_Timer3,115200);
    ENABLE_UART0_PRINTF;
  
    ENABLE_ADC_CH0;   
//	sendCommand(0x8f)	;
//	reset();
//	sendCommand(0x44);  // set single address
// strobe_low;
//  shiftOut(0xc0); // 1st digit
//  shiftOut(0xff);
//  strobe_high;
	//shiftOut(0xAA);
	//printf("0x%X\n",readButtons());
	P10_PUSHPULL_MODE;
	P11_PUSHPULL_MODE;
	P12_PUSHPULL_MODE;
    while(1)
    {
			//shiftOut(0x42);
			printf("0x%X\n",readButtons());
			//readButtons();
			//printf("hello")	;
			//shiftIn();
//      Timer2_Delay(24000000,128,300,1000);;
//      clr_ADCCON0_ADCF;
//      set_ADCCON0_ADCS;                  // ADC start trig signal
//      while(ADCF == 0);
//      temp=(ADCRH<<4)+(ADCRL&0x0F);
//      printf ("\n ADC Value = 0x%02X", temp);
//      P35 ^= 1;
    
			
    }
}