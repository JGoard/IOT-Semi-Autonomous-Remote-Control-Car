// File Name : interrupts.c 
// Description: This file contains the #defines in the program for the 
// 
// Author: Josh Goard 
// Date: September 2019 
// Compiler: Built with IAR Embedded Workbench Version: MSP430 (7.12.4) 
//===========================================================================
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>

extern volatile unsigned int ADC_Left_Thresh;
extern volatile unsigned int ADC_Right_Thresh;
extern volatile unsigned int ADC_White_Avg;
extern volatile unsigned int ADC_Black_Avg;

extern char myArray[SMALL_RING_SIZE];
extern char copyArray[SMALL_RING_SIZE];

extern int index;

unsigned int temp;

volatile unsigned int usb_rx_ring_wr;
volatile char USB_Char_Rx[SMALL_RING_SIZE];

extern char HIGH[];
extern char LOW[];

volatile unsigned int Black_Left_Avg;
volatile unsigned int Black_Right_Avg;
volatile unsigned int White_Left_Avg;
volatile unsigned int White_Right_Avg;

unsigned int line_calibration=RESET;

extern volatile char choice;


extern volatile unsigned int Choices;

extern volatile unsigned char ADC_Print[TENS];

extern char display_line[TOTAL_ROWS][NULL_LOC];
extern char *display[TOTAL_ROWS];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int ADC_Return;

extern volatile unsigned char display_changed;
extern volatile unsigned int Time_Sequence =FALSE;
extern volatile char one_time;
extern volatile unsigned int channel=FALSE;
extern volatile unsigned int check;
extern volatile unsigned int debounce_count;

extern volatile unsigned int ADC_Clock;
extern volatile unsigned int ADC_Left_Detector;
extern volatile unsigned int ADC_Right_Detector;
extern volatile unsigned char ADC_Value[STRING_MAX];

extern volatile char one_time;

extern char switchTransmit;

char trade;

int pointer;
extern int menuChoice;

//Serial Communication GLobals

volatile unsigned int command=FALSE;

extern volatile char* Rx_Received[SMALL_RING_SIZE];

extern volatile unsigned int usb_rx_ring_wr_A0;
extern volatile unsigned int usb_rx_ring_rd_A0;
extern volatile unsigned int usb_tx_ring_wr_A0;
extern volatile unsigned int usb_tx_ring_rd_A0;

extern volatile unsigned int UCA0_index;

extern volatile char USB_Char_Rx_A0[SMALL_RING_SIZE];
extern volatile char USB_Char_Tx_A0[SMALL_RING_SIZE];

extern volatile char test_command_A0[LENGTH];
//USCA1
extern volatile unsigned int usb_rx_ring_wr_A1;
extern volatile unsigned int usb_rx_ring_rd_A1;
extern volatile unsigned int usb_tx_ring_wr_A1;
extern volatile unsigned int usb_tx_ring_rd_A1;

extern char commandbuff[STRING_MAX][C]; 

extern volatile char test_command_A1[LENGTH];

extern volatile unsigned int UCA1_index;

extern char storeArray;
char clearArray=FALSE;

extern char ASCII1[];
int wait;
char IRPause;
int pause;
extern char lineDetected;

extern int rw;

extern char myFRAMArray[];
char FRAM=FALSE;
char IOTCommand=FALSE;
char oneTime=TRUE;
extern int r;

char trade1;
extern char setTime;
int stopTime;
extern int index;
char copySSID=FALSE;
char copyIP=FALSE;
char copyCommand=FALSE;
extern char goAuto;
int parse;
extern int menuSelect;
extern int ADC_Thumb;
int totalTime;
int commandTime=FALSE;
int runTime;
extern char secondlineDetected;
int commandpass=FALSE;
int batteryLevel;
char haltAuto;
int haltTime;

extern char IOTArray[];
extern int scanTime;

extern volatile char USB_Char_Rx_A1[SMALL_RING_SIZE];
extern volatile char USB_Char_Tx_A1[SMALL_RING_SIZE];

//=============================================================================

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void){
  
  //
  //TimerB0 0 Interrupt handler
  // 
  
  if(commandTime!=RESET){
    commandTime--;
  }  
  
  if(menuSelect==ADC_MENU){
    scanTime++;
  }
  
  if((lineDetected||goAuto)) {
    wait++;
  }
  
  if(IRPause){
   pause++;
  }
  
  if(haltAuto){
   haltTime++;
  }
  
  
  if((menuSelect==IOT_MENU)&&(setTime)){
    if(runTime%FULL_SEC==FALSE){
      totalTime++;
      HEXtoBCD(totalTime);
      strcpy(display_line[FINALLY], ASCII1);
      update_string(display_line[FINALLY], LOW_LOW);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
    }
  }
  
  check++;
  runTime++;
  
  
  TB0CCR0+= TB0CCR0_INTERVAL; // Add Offset to TBCCR0
  //   
}

//=================================================================//

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  //
  //TimerB0 1 2, Overflow Interrupt Vector (TBIV ) handler
  //
  switch(__even_in_range(TB0IV,CHECK_TIME_VECTOR)){
  case RESET: break; // No interrupt
  case CCR1: // CCR1 not used
    Choices++;
    TB0CCR1+= TB0CCR1_INTERVAL; // Add Offset to TBCCR1
    break;
  case CCR2: // CCR2 not used
    //...... Add What you need happen in the interrupt ......
    TB0CCR2+= TB0CCR2_INTERVAL; // Add Offset to TBCCR2
    break;
  case OVERFLOW: // overflow
    //...... Add What you need happen in the interrupt ......
    break;
  default: break;
  }
}


//=================================================================//
//Allows for Switch 1 to be used.  Remember to Debounce
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
  // Switch 1
  if (P4IFG & SW1){
    P4IFG &= ~SW1; // IFG SW2 cleared
  }
  switch(menuSelect){
  case IOT:
    menuChoice=IOT_MENU;
    break;
  case ADC:  
    menuChoice=ADC_MENU;
    break;
  case BACKLITE_OP:
    P6OUT ^= LCD_BACKLITE;
    break;
    
  default: break;
  }
  
}



//=================================================================//
////Allows for Switch 2 to be used.  Remember to Debounce

#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
  // Switch 2
  if (P2IFG & SW2) {
    P2IFG &= ~SW2; // IFG SW2 cleared  
    
    menuChoice=FALSE;
    channel=WAIT;
    
    
    
    
    
  }
}
//==========================================================================//
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
  case ADCIV_NONE:
    break;
  case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
    // before its previous conversion result was read.
    break;
  case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
    break;
  case ADCIV_ADCHIIFG: // Window comparator interrupt flags
    
    break;
  case ADCIV_ADCLOIFG: // Window comparator interrupt flag
    
    break;
  case ADCIV_ADCINIFG: // Window comparator interrupt flag
    break;
  case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
    
    
    
    ADCCTL0 &= ~ADCENC;
    
    switch(channel){
      
      case(ADC_THUMB_CHANNEL):
        
        ADCMCTL0 &= ~ADCINCH_5;
        ADCMCTL0 |= ADCINCH_3;
        ADC_Return = ADCMEM0; // Channel A5 VThumb
        ADC_Thumb = ADC_Return;
        channel++;
        break;
        
        case(L_DETECT_CHANNEL):
          ADCMCTL0 &= ~ADCINCH_3;
          ADCMCTL0 |= ADCINCH_2;
          ADC_Return = ADCMEM0;
          ADC_Left_Detector = ADC_Return;
          channel++;
          break;
          
          case(R_DETECT_CHANNEL):
            ADCMCTL0 &= ~ ADCINCH_2;
            ADCMCTL0 |= ADCINCH_4;
            ADC_Return = ADCMEM0;//returns left detect
            ADC_Right_Detector = ADC_Return;
            channel++;
            break;
            
            case(V_BAT_CHANNEL):
              ADCMCTL0 &= ~ADCINCH_4;
              ADCMCTL0 |=  ADCINCH_5;
              ADC_Return = ADCMEM0;
              batteryLevel = ADC_Return;
              channel=ADC_THUMB_CHANNEL;
              break;
              
              
              
    default: break;
    
    }
    ADCCTL0 |= ADCENC;
    ADCCTL0 |= ADCSC;
    
  default: break;
  }
  
}
//==============================================================================

//==============================================================================
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void){
  
  
  switch(__even_in_range(UCA0IV,CHECK_UART_VECTOR)){
  case RESET: // Vector 0 - no interrupt
    
    
    break;
  case RECEIVE_CASE: // Vector 2 - RXIFG
    
    
    trade=UCA0RXBUF; //per step 17: refer to lines 349-351
    
    if((trade=='"')){  //trying to find/receive the quotes for the SSID
      
      copySSID=TRUE;
      
    }
    if((trade=='r')){
      copyIP=TRUE; //indicates IOT line holds the IP Address
    }
    
    temp = usb_rx_ring_wr_A0++; //incremenets index based on size
    USB_Char_Rx_A0[temp] = trade; // RX -> USB_Char_Rx character
    
    switch(commandpass){
      
    case INITIAL_PASS:
      if (USB_Char_Rx_A0[temp]=='7'){
        commandpass=CONTINUE_PASS;
      }
      else commandpass=FALSE;
      
      break;
    case CONTINUE_PASS: 
      if (USB_Char_Rx_A0[temp]=='3'){
        commandpass=CONTINUING_PASS;
      }
      else commandpass=FALSE;
      
      break;
    case CONTINUING_PASS: 
      if (USB_Char_Rx_A0[temp]=='2'){
        commandpass=END_PASSWORD;
      }
      else commandpass=FALSE;
      
      break;
    case END_PASSWORD: 
      if (USB_Char_Rx_A0[temp]=='5'){
        copyCommand=TRUE;
        setTime=TRUE;
        rw=temp;
        
      }
      commandpass=FALSE;
      
      break;  
      
      
      break;
      
    default: 
      if (USB_Char_Rx_A0[temp]=='^'){
        commandpass=INITIAL_PASS;
        
        
      }
      
      
      
      break;  
    }
    
    
    if (usb_rx_ring_wr_A0 >= (sizeof(USB_Char_Rx_A0))){
      usb_rx_ring_wr_A0 = BEGINNING; // Circular buffer back to beginning
    }
    UCA1TXBUF=trade;//Now, we're looping this back into the UCA1 Interrupt
    
    break;
    
  default: break;
  
  // case TRANSFER_CASE: // Vector 4 – TXIFG
    
    // switch(UCA0_index++){
    // case RESET: //
    // case 1: //
    // case 2: //
    // case 3: //
    // case 4: //
    // case 5: //
    // case 6: //
    // case 7: //
         // case 8: //
           // UCA0TXBUF = USB_Char_Tx_A0[UCA0_index];
           // break;
    // case 9: //
      // UCA0TXBUF = CARRIAGE;
      // break;
    // case 10: // Vector 0 - no interrupt
      // UCA0TXBUF = NEWLINE;
      // break;
    // default:
      // UCA0IE &= ~UCTXIE; // Disable TX interrupt
      // break;
    // }
    // break;    
  // }
}


//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void){
  
  unsigned int temp;
  switch(__even_in_range(UCA1IV,CHECK_UART_VECTOR)){
  case RESET: // Vector 0 - no interrupt
    
    
    
    break;
  case RECEIVE_CASE: // Vector 2 - RXIFG
    //    oneTime=TRUE; //Done to latch myself out of line 350
    temp = usb_rx_ring_wr_A1++; //incremenets index based on size
    
    
    trade1=UCA1RXBUF;//Done for testing per step 9
    
    if((IOTCommand==FALSE)&&(trade1=='^')){ //Per ISR Iteration, determines whether
      FRAM=TRUE; //IOT init as FALSE. 
      IOTCommand=TRUE;
    }
    
    else if(IOTCommand==FALSE){ //IOT Command or not, outputs to USCA0 if so
      UCA0TXBUF=trade1;   // per step 17 USES A LOOPBACK , testing to see if it receives
    }
    
    if(FRAM==TRUE){
      UCA1TXBUF=trade1;
    }
    
    if(trade1!='\0'){
      switchTransmit=TRUE; //Switch case function in main, will use for FRAM Commands
    }
    else if(trade1=='\r'){
      switchTransmit=FALSE;
    }
    
    USB_Char_Rx_A1[temp] = trade1; // RX -> USB_Char_Rx character
    
    if (usb_rx_ring_wr_A1 >= (sizeof(USB_Char_Rx_A1))){
      usb_rx_ring_wr_A1 = BEGINNING; // Circular buffer back to beginning
    }
    
    break;
    
  case TRANSFER_CASE:  
    
    // switch(UCA1_index++){
      
    // case 0: //
    // case 1: //
    // case 2: //
    // case 3: //
    // case 4: //
    // case 5: //
    // case 6: //
    // case 7: //
      // //    case 8: //
      // //      UCA1TXBUF = USB_Char_Tx_A1[UCA1_index];
      // //      break;
    // case 9: //
      // UCA1TXBUF = CARRIAGE;
      // break;
    // case 10: // Vector 0 - no interrupt
      // UCA1TXBUF = NEWLINE;
      // break;
    // default:
      // UCA1IE &= ~UCTXIE; // Disable TX interrupt
      // break;
      // break;
    // }
  }
}
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------


