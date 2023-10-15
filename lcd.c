/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* SPDX-License-Identifier: Apache-2.0                                                                     */
/* Copyright(c) 2020 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/


/************************************************************************************************************/
/*  File Function: MS51 DEMO project                                                                        */
/************************************************************************************************************/

#include "MS51_16K.h"
#include "stdio.h"
/*  
   LCD16x2 4 bit 8051 interface
   http://www.electronicwings.com
*/

#define LCD_4BIT_1_LINE_5_x_7            0x20
#define LCD_4BIT_2_LINE_5_x_7            0x28
#define LCD_8BIT_1_LINE_5_x_7            0x30
#define LCD_8BIT_2_LINE_5_x_7            0x38
#define LCD_ENTRY_MODE                   0x06
#define LCD_DISPLAY_OFF_CURSOR_OFF       0x08
#define LCD_DISPLAY_ON_CURSOR_ON         0x0E
#define LCD_DISPLAY_ON_CURSOR_OFF        0x0C
#define LCD_DISPLAY_ON_CURSOR_BLINKING   0x0F
#define LCD_DISPLAY_SHIFT_ENTIRE_LEFT    0x18
#define LCD_DISPLAY_SHIFT_ENTIRE_RIGHT   0x1C
#define LCD_MOVE_CURSOR_LEFT_ONE_CHAR    0x10
#define LCD_MOVE_CURSOR_RIGHT_ONE_CHAR   0x14
#define LCD_CLEAR_ALSO_DDRAM             0x01
#define poort 1
unsigned char mbyte;  
unsigned char mbyte1; 
unsigned char mbyte2; 
  // Character array to store the result

sfr LCD_Port=0x80;		/* P0 port as data port */
sbit rs=P0^0;			/* Register select pin */
sbit rw=P0^1;			/* Read/Write pin */
sbit en=P0^2;			/* Enable pin */

unsigned int state=0;

/* Function to provide delay Approx 1ms with 11.0592 Mhz crystal*/
void delay(unsigned int count)    		
{
     int i,j;
     for(i=0;i<count;i++)
     for(j=0;j<112;j++);
}

void LCD_Command (char cmnd)	/* LCD16x2 command funtion */
{
	LCD_Port =(LCD_Port & 0x0F) | (cmnd & 0xF0);/* Send upper nibble */
	rs=0;			/* Command reg. */
	rw=0;			/* Write operation */
	en=1; 
	delay(1);
	en=0;
	delay(2);
	
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);/* Send lower nibble */
	en=1;			/* Enable pulse */
	delay(1);
	en=0;
	delay(5);
}

void LCD_Char (char char_data)  /* LCD data write function */
{
	LCD_Port =(LCD_Port & 0x0F) | (char_data & 0xF0);/* Send upper nibble */
	rs=1;  			/*Data reg.*/
	rw=0;  			/*Write operation*/
	en=1;  
	delay(1);
	en=0;
	delay(2);

	LCD_Port = (LCD_Port & 0x0F) | (char_data << 4);/* Send lower nibble */
	en=1;  			/* Enable pulse */
	delay(1);
	en=0;
	delay(5);

}



void LCD_String (char *str)	/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)  /* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);  /* Call LCD data write */
	}
}

void LCD_String_xy (char row, char pos, char *str)  /* Send string to LCD function */
{
	if (row == 0)
	LCD_Command((pos & 0x0F)|0x80);
	else if (row == 1)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str);  	/* Call LCD string function */
}

void LCD_String_xy1 (char row, char pos, char *str, char *str1)  /* Send string to LCD function */
{	LCD_Command(0x14);
	LCD_String(str);
	if (row == 0)
	LCD_Command((pos & 0x0F)|0x80);
	else if (row == 1)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str1);  	/* Call LCD string function */
	//Timer3_Delay(24000000,16,3000,60);
}

void LCD_Init (void)		/* LCD Initialize function */
{
	delay(20);		/* LCD Power ON Initialization time >15ms */
	LCD_Command (0x02);	/* 4bit mode */
	LCD_Command (0x28);	/* Initialization of 16X2 LCD in 4bit mode */
	LCD_Command (0x0E);	/* Display ON Cursor OFF */
	LCD_Command (0x06);	/* Auto Increment cursor */
	LCD_Command (0x01);	/* clear display */
	LCD_Command (0x80);	/* cursor at home position */
}

///my functions
/*
move cursor using a gpio
*/
void cursor_move(void)//it works with input
	{	
		int x=0;
		LCD_Command (0x80);	
		while(x<15){
			mbyte= P13;
			P12=mbyte;
			if(mbyte==0){
				LCD_Command(0x14);
				x++;
				Timer3_Delay(24000000,16,3000,100);
				mbyte=0;
			}
		}
//		LCD_Command(LCD_MOVE_CURSOR_RIGHT_ONE_CHAR);
//		Timer3_Delay(24000000,16,3000,200);
//		//P13=1;
//		if(P13 ==1){
//			state^=poort;  //xor operation
//			P12=state;
//			LCD_Command(0x14);
//			P13=0;
//			Timer3_Delay(24000000,16,3000,200);
//			//P12|=P13;			
		}
//	}
//	
		/*about below function
		its made this way so that if there are 3 strings in array it iterates for 
			3 times so that and the nxt row ill be blank this way the user will understand
			it is the end of the line
		function: To display many lines of code using 2 rows(input is array)
		*/
void scroll(char *arr[]) //used in settings only(they told noo need (U_U))
	{	
			//char *arr[] = {"one", "two", "three","four"};
		
   int x= sizeof(arr);
		int y, i
			;
			if(x%2==0)
				{
					y=(x/2);
			}
				else{
					y=(x/2)+1;
				}
  for(i=0; i<x; i++) 
  {
		LCD_Command(0x01);
		LCD_String_xy1(1,0,arr[i],arr[i+1]);
		Timer3_Delay(24000000,16,3000,300);
}
	}

	void scroll_rev(char *arr[]) //used in settings only
	{	
			//char *arr[] = {"one", "two", "three","four"};
		
   int x= sizeof(arr);
		int y, i
			;
			if(x%2==0)
				{
					y=(x/2);
			}
				else{
					y=(x/2)+1;
				}
  if(x%2==0)
		{
			for(i=0; i<x; i++) 
  {
		LCD_Command(0x01);
		LCD_String_xy1(1,0,arr[x-i-1],arr[x-i]);
		Timer3_Delay(24000000,16,3000,300);
}
	}
		else
		{
			for(i=0; i<x-1; i++) 
  {
		LCD_Command(0x01);
		LCD_String_xy1(1,0,arr[x-i-2],arr[x-i-1]);
		Timer3_Delay(24000000,16,3000,300);
}
		}
	}
void navi_cur(void)// to navigate through the displayed string
	{
		LCD_Command(0x0F);
		LCD_Command (0x80);	
		LCD_Command(0x14);
		Timer3_Delay(24000000,16,3000,200);
		LCD_Command(0x14);
		Timer3_Delay(24000000,16,3000,200);
		LCD_Command(0x14);
		Timer3_Delay(24000000,16,3000,200);
		LCD_Command(0x14);
		Timer3_Delay(24000000,16,3000,200);
	}

void navi_cur_input(void)
	{
		
		while(1){
			mbyte= P13;
			P12=mbyte;
			if(mbyte==0){
				LCD_Command(0x14);
				Timer3_Delay(24000000,16,3000,200);
				mbyte=0;
			}
	
		}}

//scroll with input
void scroll1(char *arr[]) 
	{	
   int x= sizeof(arr)/sizeof(arr[0]);
		int i;
		for(i=0; i<x; i++) 
		{
			mbyte1=P13;
			P12=mbyte1;
			while(mbyte1==0){		
		LCD_Command(0x01);
		LCD_String_xy1(1,0,arr[i],arr[i+1]);
    // Convert 'num' to a string and store it in 'charNum'
   // sprintf(charNum, "%d", i);
		
		//LCD_String(charNum);
		Timer3_Delay(24000000,16,3000,300);
				mbyte1=1;
			}
}
	}
	
	void scroll2(char *arr[]) //scroll works up and down for even numbered elements
	{	
		char charNum[1];
		int x=3; //sizeof(arr)/sizeof(arr[0]);
		int i;
		sprintf(charNum, "%d", x);  
			
		while(i<=x+1)
			{
				mbyte1=P13;
				//P12=mbyte1;
				mbyte2=P14;
				P12=mbyte2;
				while((mbyte1==0) && (i<=x))
					{
						LCD_Command(0x01);
						LCD_String_xy1(1,0,arr[i],arr[i+1]);
						LCD_String(charNum);
						i=i+2;
						mbyte1=1;
						mbyte2==1;
						Timer3_Delay(24000000,16,3000,300);
					}
				while((mbyte2==0) &&(i>=2))
				{
					LCD_Command(0x01);
					LCD_String_xy1(1,0,arr[i-2],arr[i-1]);
					i=i-2;  //i sud put this the line before and check
					mbyte2==1;
					Timer3_Delay(24000000,16,3000,300);
				}
			}
	}
	
	void scroll3(char *arr[], int size) //scroll in progress for odd up and down
	{	
		char charNum[1];
		int y=size; //sizeof(arr)/sizeof(arr[0]);
		int i;
		while(i<=y+1)
			{
				mbyte1=P13;
				P12=mbyte1;
				mbyte2=P14;
				//P12=mbyte2;
				while((mbyte1==0) && (i<=y)){
				if(i==(y-1))
				{
						LCD_Command(0x01);
						LCD_String(arr[y-1]);
						Timer3_Delay(24000000,16,3000,300);		
					i=i+1;
					
				}
				else
					{
						LCD_Command(0x01);
						LCD_String_xy1(1,0,arr[i],arr[i+1]);
						 sprintf(charNum, "%d", y);
						LCD_String(charNum);
						//LCD_String(arr[x-1]);
						i=i+2;
						mbyte1=1;
						mbyte2==1;
						Timer3_Delay(24000000,16,3000,300);		
				}}
				
				
				
//				while((mbyte2==0) &&(i>=2))// for downwards
//				{
//					LCD_Command(0x01);
//					LCD_String_xy1(1,0,arr[i-2],arr[i-1]);
//					i=i-2;
//					mbyte2==1;
//					Timer3_Delay(24000000,16,3000,300);
//				}
			}
	}
	
	
	void scroll4(char *arr[], int size)  //scroll(still on progress) will limk with the array 
	{	
   //int x= sizeof(arr)/sizeof(arr[0]);
		int i;
		for(i=0; i<size; i++) 
		{
			mbyte1=P13;
			P12=mbyte1;
			while(mbyte1==0){		
		LCD_Command(0x01);
		LCD_String_xy1(1,0,arr[i],arr[i+1]);
    // Convert 'num' to a string and store it in 'charNum'
   // sprintf(charNum, "%d", i);
		
		//LCD_String(charNum);
		Timer3_Delay(24000000,16,3000,300);
				mbyte1=1;
			}
}
	}
	
	void scroll5(char *myStrings[][2], int size) //vidaamuyarchi ^0^ it works 
	{
		int size1=size;
		// Define an array of strings
    /*char *myStrings[][2] = {
        {"String 1", "one"},
        {"String 2","two"},
        {"String 3","three"},
        {"String 4","four"}
    };*/

    // Initialize input bits for traversal control
    bit forwardBit = 0; // Input bit for forward traversal
    bit backwardBit = 0; // Input bit for backward traversal

    // Initialize traversal index
    int currentIndex = 0;
		P13=1;
    P14=1;
    // Main loop
    while (1) {
        // Check input bits for traversal direction
        
        forwardBit=P13;
        P12=P13;
        backwardBit=P14;
        if (forwardBit==0) {
            // Move forward in the array
            //currentIndex++;
            if (currentIndex < (size1)) //3 is size -1
							{
                currentIndex++;
											LCD_Command(0x01);
				LCD_Command(0x14);
				//LCD_String_xy1(myStrings[currentIndex][0],myStrings[currentIndex][1]);
				LCD_String_xy1(1,0,myStrings[currentIndex][0],myStrings[currentIndex][1]);
				P13=1;
				P14=1;
				Timer3_Delay(24000000,16,3000,100);
                //currentIndex = 0; // Wrap around if at the end
            }
        } else if (backwardBit==0) // 0 is the least index
				{
            // Move backward in the array
            if (currentIndex > 0) {
                currentIndex--; 
			LCD_Command(0x01);
				LCD_Command(0x14);
				//LCD_String_xy1(myStrings[currentIndex][0],myStrings[currentIndex][1]);
				LCD_String_xy1(1,0,myStrings[currentIndex][0],myStrings[currentIndex][1]);
				P13=1;
				P14=1;
				Timer3_Delay(24000000,16,3000,100);							
                //currentIndex = sizeof(myStrings) / sizeof(myStrings[0]) - 1; // Wrap around if at the beginning
            }
        }
        // Display the current string on the LCD
				//Timer3_Delay(24000000,16,3000,60);
	
        // Delay or debounce as needed
        // You may need to implement a delay to control the speed of traversal

        // Add any additional logic or code here
    }
		
	}
	
	void scroll52(char *myStrings[], int size) //vidaamuyarchi single smooth scroll //lessgo it works
	{
		int size1=size;
		// Define an array of strings
    /*char *myStrings[][2] = {
        {"String 1", "one"},
        {"String 2","two"},
        {"String 3","three"},
        {"String 4","four"}
    };*/

    // Initialize input bits for traversal control
    bit forwardBit = 0; // Input bit for forward traversal
    bit backwardBit = 0; // Input bit for backward traversal

    // Initialize traversal index
    int currentIndex = 0;
		P13=1;
    P14=1;
    // Main loop
    while (1) {
        // Check input bits for traversal direction
        
        forwardBit=P13;
        P12=P13;
        backwardBit=P14;
        if (forwardBit==0) {
            // Move forward in the array
            //currentIndex++;
            if (currentIndex < (size1)) //3 is size -1
							{
                
										LCD_Command(0x01);
									
				LCD_Command(0x14);
								LCD_String(">");

				//LCD_String_xy1(myStrings[currentIndex][0],myStrings[currentIndex][1]);
				LCD_String_xy1(1,2,myStrings[currentIndex],myStrings[currentIndex+1]);
				currentIndex++;
				P13=1;
				P14=1;
				Timer3_Delay(24000000,16,3000,100);
                //currentIndex = 0; // Wrap around if at the end
            }
        } else if (backwardBit==0) // 0 is the least index
				{
            // Move backward in the array
            if (currentIndex > 1) {
                currentIndex--; 
			LCD_Command(0x01);							
				LCD_Command(0x14);
							LCD_String(">");
				//LCD_String_xy1(myStrings[currentIndex][0],myStrings[currentIndex][1]);
				LCD_String_xy1(1,2,myStrings[currentIndex-1],myStrings[currentIndex]);
				P13=1;
				P14=1;
				Timer3_Delay(24000000,16,3000,100);							
               
            }
        }
    }
		
	}
	
//	
//	/////////////////scroll 5 with debounce generated with chatgpt didnt work (T_T)
//	
//	bit ReadGPIOWithDebounce(int pin) {
//    // Implement debounce logic and return the state of the GPIO pin
//    // Example debounce logic: Read multiple times and return a stable value
//    // You may need to add a delay between readings
//    unsigned char debounceCount = 0;
//		unsigned int delay;
//    bit previousState = 0;
//    bit currentState = 0;
//    
//    do {
//        previousState = currentState;
//        currentState = P1 & (1 << pin); // Replace with actual GPIO pin reading
//        
//        // Debounce by requiring consecutive readings to be the same
//        if (previousState == currentState) {
//            debounceCount++;
//        } else {
//            debounceCount = 0;
//        }
//        
//        // Add a small delay between readings
//        // You may need to adjust the delay based on your hardware and requirements
//        for (delay = 0; delay < 1000; delay++) {
//            // Delay loop
//        }
//        
//    } while (debounceCount < 5); // Adjust the count threshold as needed for your application
//    
//    return currentState;
//}

//void scroll51(void) {
//    // Define an array of strings
//    char *myStrings[][2] = {
//        {"String 1", "one"},
//        {"String 2", "two"},
//        {"String 3", "three"},
//        {"String 4", "four"}
//    };

//    // Initialize input bits for traversal control
//    bit forwardBit = 1; // Initial state for forward traversal (no debounce needed)
//    bit backwardBit = 1; // Initial state for backward traversal (no debounce needed)

//    // Initialize traversal index
//    int currentIndex = 0;

//    // Main loop
//    while (1) {
//        // Read GPIO input with debounce for forwardBit (P13)
//        if (forwardBit == 1) {
//            forwardBit = ReadGPIOWithDebounce(3); // Replace with your GPIO pin number
//        }

//        // Read GPIO input with debounce for backwardBit (P14)
//        if (backwardBit == 1) {
//            backwardBit = ReadGPIOWithDebounce(4); // Replace with your GPIO pin number
//        }

//        // Check for a rising edge of forwardBit
//        if (forwardBit == 0) {
//            // Move forward in the array
//            if (currentIndex < 3) {
//                currentIndex++;
//            }
//        }

//        // Check for a rising edge of backwardBit
//        if (backwardBit == 0) {
//            // Move backward in the array
//            if (currentIndex > 0) {
//                currentIndex--;
//            }
//        }

//        // Display the current string on the LCD
//        LCD_String_xy1(1, 0, myStrings[currentIndex][0], myStrings[currentIndex][1]);

//        // Delay or debounce as needed
//        // You may need to implement a delay to control the speed of traversal

//        // Add any additional logic or code here
//    }
//}