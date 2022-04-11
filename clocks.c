// ------------------------------------------------------------------------------
//
//  Description: This file contains the Clock Initialization
//
//	Josh Goard
//  Sept 2019
//  Built with IAR Embedded Workbench Version: V7.12.1
// ------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"

// MACROS========================================================================
#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)





void Init_Clocks(void){
  // -----------------------------------------------------------------------------
  // Clock Configurtaions
  // This is the clock initialization for the program.
  // Initial clock configuration, runs immediately after port configuration.
  // Disables 1ms watchdog timer,
  // Configure MCLK for 8MHz and XT1 sourcing ACLK and FLLREF.
  //
  // Description: Configure ACLK = 32768Hz,
  //                        MCLK = DCO + XT1CLK REF = 8MHz,
  //                        SMCLK = MCLK/2 = 4MHz.
  // Toggle LED to indicate that the program is running.
  //
  // -----------------------------------------------------------------------------
  WDTCTL = WDTPW | WDTHOLD;  // Disable watchdog
  
  do{
    CSCTL7 &= ~XT1OFFG;      // Clear XT1 fault flag
    CSCTL7 &= ~DCOFFG;       // Clear DCO fault flag
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG); // Test oscillator fault flag
  __bis_SR_register(SCG0);   // disable FLL
  
  CSCTL1 = DCOFTRIMEN_1;
  CSCTL1 |= DCOFTRIM0;
  CSCTL1 |= DCOFTRIM1;       // DCOFTRIM=3
  CSCTL1 |= DCORSEL_3;       // DCO Range = 8MHz
  
  CSCTL2 = FLLD_0 + EIGHT_MHZ;     // DCODIV = 8MHz
  
  CSCTL3 |= SELREF__XT1CLK;  // Set XT1CLK as FLL reference source
  __delay_cycles(HUNDREDS);
  __bic_SR_register(SCG0);   // enable FLL
  Software_Trim();           // Software Trim to get the best DCOFTRIM value
  
  CSCTL4 = SELA__XT1CLK;     // Set ACLK = XT1CLK = 32768Hz
  CSCTL4 |= SELMS__DCOCLKDIV;// DCOCLK = MCLK and SMCLK source
  
  CSCTL5 |= DIVM_0;          // MCLK = DCOCLK = 8MHZ,
  CSCTL5 |= DIVS_0;          // SMCLK = DCOCLK = 8MHz
  
  PM5CTL0 &= ~LOCKLPM5;      // Disable the GPIO power-on default high-impedance mode
  // to activate previously configured port settings
}

void Software_Trim(void){
  // --COPYRIGHT--,BSD_EX
  // Copyright (c) 2014, Texas Instruments Incorporated
  // All rights reserved.
  unsigned int oldDcoTap = CLEAR;
  unsigned int newDcoTap = CLEAR;
  unsigned int newDcoDelta = CLEAR;
  unsigned int bestDcoDelta = CLEAR;
  unsigned int csCtl0Copy = FALSE;
  unsigned int csCtl1Copy = FALSE; 
  unsigned int csCtl0Read = FALSE;
  unsigned int csCtl1Read = FALSE;
  unsigned int dcoFreqTrim = HUNDREDS;
  unsigned char endLoop = FALSE;
  do{
    CSCTL0 = DCOTAP;                         // DCO Tap = 256
    do{
      CSCTL7 &= ~DCOFFG;                    // Clear DCO fault flag
    }while (CSCTL7 & DCOFFG);               // Test DCO fault flag
    // Wait FLL lock status (FLLUNLOCK) to be stable
    // Suggest to wait 24 cycles of divided FLL reference clock
    __delay_cycles((unsigned int)CYCLES * MCLK_FREQ_MHZ);
    while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) &&
          ((CSCTL7 & DCOFFG) == FALSE));
    csCtl0Read = CSCTL0;                    // Read CSCTL0
    csCtl1Read = CSCTL1;                    // Read CSCTL1
    oldDcoTap = newDcoTap;                  // Record DCOTAP value of last time
    newDcoTap = csCtl0Read & HEX_NUMBER;        // Get DCOTAP value of this time
    dcoFreqTrim = (csCtl1Read & DCOFTRIM)>>SHIFT_4; // Get DCOFTRIM value
    if(newDcoTap < EIGHT_BIT){                    // DCOTAP < 256
      newDcoDelta = EIGHT_BIT - newDcoTap;        // Delta value between DCPTAP and 256
      if((oldDcoTap != CLEAR) &&
         (oldDcoTap >= EIGHT_BIT)){               // DCOTAP cross 256
           endLoop = TRUE;                        // Stop while loop
         }else{
           dcoFreqTrim--;
           CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<SHIFT_4);
         }
    }else{                                  // DCOTAP >= 256
      newDcoDelta = newDcoTap - EIGHT_BIT;        // Delta value between DCPTAP and 256
      if(oldDcoTap < EIGHT_BIT){                  // DCOTAP cross 256
        endLoop = TRUE;                        // Stop while loop
      }else{
        dcoFreqTrim++;
        CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
      }
    }
    if(newDcoDelta < bestDcoDelta){         // Record DCOTAP closest to 256
      csCtl0Copy = csCtl0Read;
      csCtl1Copy = csCtl1Read;
      bestDcoDelta = newDcoDelta;
    }
  }while(endLoop == FALSE);                     // Poll until endLoop == 1
  CSCTL0 = csCtl0Copy;                      // Reload locked DCOTAP
  CSCTL1 = csCtl1Copy;                      // Reload locked DCOFTRIM
  while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));// Poll until FLL is locked
}


