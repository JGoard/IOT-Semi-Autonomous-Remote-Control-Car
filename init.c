//=========================================================================== 
// File Name : init.c 
// 
// Description: This file contains the Initialization for all port pins 
// 
// Author: Josh Goard 
// Date: September 2019 
// Compiler: Built with IAR Embedded Workbench Version: MSP430 (7.12.4) 
//===========================================================================
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>

extern char display_line[TOTAL_ROWS][NULL_LOC];
extern char *display[TOTAL_ROWS];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int ADC_Return1;

//Serial Communication Globals
//Serial Communication GLobals

extern int onetime;


 extern volatile unsigned int usb_rx_ring_wr_A0;
 extern volatile unsigned int usb_rx_ring_rd_A0;
 extern volatile unsigned int usb_tx_ring_wr_A0;
 extern volatile unsigned int usb_tx_ring_rd_A0;

 extern volatile unsigned int UCA0_index;

 extern volatile char USB_Char_Rx_A0[SMALL_RING_SIZE];
 extern volatile char USB_Char_Tx_A0[LARGE_RING_SIZE];

  extern volatile char test_command_A0[LENGTH];
 //USCA1
extern volatile unsigned int usb_rx_ring_wr_A1;
extern volatile unsigned int usb_rx_ring_rd_A1;
extern volatile unsigned int usb_tx_ring_wr_A1;
extern volatile unsigned int usb_tx_ring_rd_A1;

extern volatile char test_command_A1[LENGTH];

  volatile unsigned int UCA1_index;
  
  char switchTransmit;


extern volatile char USB_Char_Rx_A1[SMALL_RING_SIZE];
extern volatile char USB_Char_Tx_A1[LARGE_RING_SIZE];




void Init_Conditions(void){
  //------------------------------------------------------------------------------
  int i;
  
  for(i=RESET;i<NULL_LOC;i++){
    display_line[TOP_LINE][i] = RESET_STATE;
    display_line[TOP_MID][i] = RESET_STATE;
    display_line[MID_LOW][i] = RESET_STATE;
    display_line[LOW_LOW][i] = RESET_STATE;
  }
  display_line[TOP_LINE][MAX] =ERASE;
  display_line[TOP_MID][MAX] = ERASE;
  display_line[MID_LOW][MAX] = ERASE;
  display_line[LOW_LOW][MAX] = ERASE;
  
  display[TOP_LINE] = &display_line[TOP_LINE][ERASE];
  display[TOP_MID] = &display_line[TOP_MID][ERASE];
  display[MID_LOW] = &display_line[MID_LOW][ERASE];
  display[LOW_LOW] = &display_line[LOW_LOW][ERASE];
  update_display = FALSE;
  update_display_count = FALSE;
  // Interrupts are disabled by default, enable them.
  enable_interrupts();
  //------------------------------------------------------------------------------
}

void Init_ADC(void){
  //------------------------------------------------------------------------------
  // V_DETECT_L (0x04) // Pin 2 A2
  // V_DETECT_R (0x08) // Pin 3 A3
  // V_THUMB (0x20) // Pin 5 A5
  //------------------------------------------------------------------------------
  // ADCCTL0 Register
  ADCCTL0 = RESET; // Reset
  ADCCTL0 |= ADCSHT_1; // 16 ADC clocks
  ADCCTL0 |= ADCMSC; // MSC
  ADCCTL0 |= ADCON; // ADC ON
  // ADCCTL1 Register
  ADCCTL1 = RESET; // Reset
  ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
  ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process
  // ADCCTL2 Register
  ADCCTL2 = RESET; // Reset
  ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
  ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
  ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
  ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
  // ADCMCTL0 Register
  ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
  ADCMCTL0 |= ADCINCH_5; // V_THUMB (0x20) Pin 5 A5
  ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
  ADCCTL0 |= ADCENC; // ADC enable conversion.
  ADCCTL0 |= ADCSC; // ADC start conversion.
 
}
// Used to intilize, transmit, And receive characters at 
// A bit rate asynchronous to another device


//==============================================================================

void Init_Serial_UCA0(char choice){
int i;
for(i=RESET; i<SMALL_RING_SIZE; i++){
	USB_Char_Rx_A0[i] = RESET; // USB Rx Buffer
}
usb_rx_ring_wr_A0 = BEGINNING;
usb_rx_ring_rd_A0 = BEGINNING;

if(onetime==FALSE){
for(i=RESET; i<LARGE_RING_SIZE; i++){ // May not use this
	USB_Char_Tx_A0[i] = RESET; // USB Tx Buffer
}
  onetime=TRUE;
}

usb_tx_ring_wr_A0 = BEGINNING;
usb_tx_ring_rd_A0 = BEGINNING;

// Configure UART 1
UCA0CTLW0 = RESET; // Use word register
UCA0CTLW0 |= UCSWRST; // Set Software reset enable
UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
//UCA1CTLW0 |= UCA1CTL1;

 UCA0BRW = BAUD_INIT; // 115,200 Baud
// UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
// UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
UCA0MCTLW = FAST_BAUD_RATE; 




UCA0CTLW0 &= ~ UCSWRST; // Set Software reset enable
UCA0IE |= UCRXIE; 
}
//==============================================================================

void Init_Serial_UCA1(char choice){
int i;
switchTransmit=FALSE;
for(i=RESET; i<SMALL_RING_SIZE; i++){
	USB_Char_Rx_A1[i] = RESET; // USB Rx Buffer
}
usb_rx_ring_wr_A1 = BEGINNING;
usb_rx_ring_rd_A1 = BEGINNING;

if(onetime==FALSE){
for(i=RESET; i<LARGE_RING_SIZE; i++){ // May not use this
	USB_Char_Tx_A1[i] = RESET; // USB Tx Buffer
}
  onetime=TRUE;
}

usb_tx_ring_wr_A1 = BEGINNING;
usb_tx_ring_rd_A1 = BEGINNING;

// Configure UART 1
UCA1CTLW0 = RESET; // Use word register
UCA1CTLW0 |= UCSWRST; // Set Software reset enable
UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
UCA1BRW = BAUD_INIT; // 115200 Baud
UCA1MCTLW = FAST_BAUD_RATE;

UCA1CTLW0 &= ~ UCSWRST; // Set Software reset enable
UCA1IE |= UCRXIE; 
}


