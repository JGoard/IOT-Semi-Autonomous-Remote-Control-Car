//----------------------------------------------------------
//
//  Description: This file contains conversion codes for the ADC Channel
//	Values
//
//
//  Josh Goard
//  Oct 2019
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>

extern volatile unsigned int ADC_Value[HUNDREDS];

extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Return;
extern volatile unsigned int ADC_Return1;
extern volatile unsigned int ADC_Return2;
extern int batteryLevel;
char* V_Thumb;



extern volatile unsigned int Timer=FALSE;


char ASCII[LENGTH];

extern volatile unsigned int channel;

char ASCII1[LENGTH];

char Left_Value [LENGTH];
char Thumb_Try [LENGTH];
char Right_Value[LENGTH];
char batteryRead[LENGTH];
char const* bit;

extern char display_line[TOTAL_ROWS][NULL_LOC];
extern char *display[TOTAL_ROWS];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;

extern volatile unsigned char display_changed;
int value;




//------------------------------------------------------------------------------
//******************************************************************************
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//------------------------------------------------------------------------------
void HEXtoBCD(int hex_value){
  
  for (int i=RESET;i<TENTHS;i++){
   ASCII1[i]='0'; 
  }

  hex_value>>=SHIFT_2;
  ASCII1[TENTHS]='\0';
  while (hex_value > THIRD){
    hex_value = hex_value - THOUSANDTHS;
    value = value + ONES;
    ASCII1[FALSE] = ASCII_OFF + value;
  }
  value = FALSE;
  while (hex_value > SECOND){
    hex_value = hex_value - HUNDRETHS;
    value = value + ONES;
    ASCII1[ONES] = ASCII_OFF + value;
  }
  value = FALSE;
  while (hex_value > FIRST){
    hex_value = hex_value - TENTHS;
    value = value + ONES;
    ASCII1[THOUSAND] = ASCII_OFF + value;
  }
  ASCII1[HUNDREDS] = ASCII_OFF + hex_value;
  
    
  
  
}
//******************************************************************************
//------------------------------------------------------------------------------



void out_character(char character){
  //------------------------------------------------------------------------------
  // The while loop will stall as long as the Flag is not set [port is busy]
  while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
  UCA0TXBUF = character;
  //------------------------------------------------------------------------------
}



//------------------------------------------------------------------------------
//******************************************************************************
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//------------------------------------------------------------------------------

//******************************************************************************
//------------------------------------------------------------------------------






