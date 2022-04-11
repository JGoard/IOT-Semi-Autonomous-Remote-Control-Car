//----------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Josh Goard
//  
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>




// Global Variables
volatile char slow_input_down;
extern char display_line[TOTAL_ROWS][NULL_LOC];
extern char *display[TOTAL_ROWS];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char const* change;
int hold_motors=RESET;
volatile unsigned extern int check;
extern volatile unsigned int ADC_Return=FALSE;
extern volatile unsigned int ADC_Return1=FALSE;
extern volatile unsigned int ADC_Value[LOW_LOW];
extern volatile unsigned int ADC_Return2=FALSE;
extern volatile unsigned char ADC_Print[DISPLAY_LENGTH];
extern volatile unsigned int Choices=FALSE;

volatile char one_time;

extern volatile unsigned int debounce_count=FALSE;

int menuChoice=FALSE;

extern volatile unsigned int channel;

extern int White_Left_Avg;
extern int White_Right_Avg;
extern int Black_Left_Avg;
extern int Black_Right_Avg;

extern  unsigned int ADC_Thumb=FALSE;
extern volatile unsigned int ADC_Left_Detector;
extern volatile unsigned int ADC_Right_Detector;
extern volatile unsigned int ADC_Left_Thresh;
extern volatile unsigned int ADC_Right_Thresh;


//=========================================================//
//Serial Communication

extern char ASCII1[DISPLAY_LENGTH];
extern char V_Thumb;
extern volatile unsigned int UCA0_index=FALSE;
extern unsigned int UCA1_index;
char myFRAMArray[SMALL_RING_SIZE];
char IOTArray[SMALL_RING_SIZE];
char commandArray[SMALL_RING_SIZE];


volatile char* Rx_Received[SMALL_RING_SIZE];

extern unsigned int temp;

extern volatile unsigned int usb_rx_ring_wr_A0=FALSE;
extern volatile unsigned int usb_rx_ring_rd_A0=FALSE;
extern volatile unsigned int usb_tx_ring_wr_A0=FALSE;
extern volatile unsigned int usb_tx_ring_rd_A0=FALSE;

volatile char test_command_A0[LENGTH];

char USB_Char_Rx_A0[SMALL_RING_SIZE];
char USB_Char_Tx_A0[SMALL_RING_SIZE];
//USCA1
extern volatile unsigned int usb_rx_ring_wr_A1=FALSE;
extern volatile unsigned int usb_rx_ring_rd_A1=FALSE;
extern volatile unsigned int usb_tx_ring_wr_A1=FALSE;
extern volatile unsigned int usb_tx_ring_rd_A1=FALSE;

volatile char test_command_A1[LENGTH];

int index=RESET;

int printCommand=RESET;

volatile char USB_Char_Rx_A1[SMALL_RING_SIZE];
char USB_Char_Tx_A1[LARGE_RING_SIZE];

volatile char choice=FALSE;
unsigned int read=RESET;
int r=RESET;

int state=FALSE;

extern char lineDetected;

extern char switchTransmit;

int onetime=FALSE;
int oneMore=FALSE;
char firstSym;
char lastSym;

extern char goAuto;
extern char haltAuto;

char leftFlag;
char rightFlag;

char received;
char waiting=TRUE;
char transmit;
extern char clearArray;
extern char IOTCommand;
extern char FRAM;
char secondlineDetected;

extern char trade;

char mainState=FALSE;
int setTime;

char storeArray=FALSE;

extern char oneTime;

const char Thumb[LENGTH]=" Pot \0";

int scanTime=FALSE;

extern int wait;

int parsing=TRUE;
extern char copySSID;
char catchLeft;
char catchRight;
extern char copyIP;
extern char copyCommand;
extern int totalTime;
extern int pause;

extern int haltTime;
int menuSelect=FALSE;

char pin[]="7325";
extern char currentCommand[COMMAND];
int commandAct=FALSE;

//========================================================//
void main(void){
  //------------------------------------------------------------------------------
  // Main Program
  // This is the main routine for the program. Execution of code starts here.
  // The operating system is Back Ground Fore Ground.
  //
  //------------------------------------------------------------------------------
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  
  PM5CTL0 &= ~LOCKLPM5;
  Init_Ports();     
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timer_B0();
  Init_Timer_B3();
  Init_LCD();                          // Initialize LCD
  Init_ADC();
  Init_Serial_UCA0(choice);
  Init_Serial_UCA1(choice);
  // Place the contents of what you want on the display, in between the quotes
  // Limited to 10 characters per line
  //
  
  strcpy(display_line[INIT], "          ");
  update_string(display_line[INIT], TOP_LINE);
  strcpy(display_line[NEXT], "          ");
  update_string(display_line[NEXT], TOP_MID);
  strcpy(display_line[CMON], "          ");
  update_string(display_line[CMON], MID_LOW);
  strcpy(display_line[FINALLY], "           ");
  update_string(display_line[FINALLY], LOW_LOW);
  Display_Update(FALSE,FALSE,FALSE,FALSE);
  
  //------------------------------------------------------------------------------
  // Begining of the "While" Operating System
  //------------------------------------------------------------------------------
  
  while(ALWAYS) {   
    
    //==============================================================================  
    if(copySSID){
      findSSID(); //Finds and display SSID
    }
    
    if(copyIP){
      findIP();  //Think about it...
    }
    
    if(copyCommand){      //Mainly used to communicate with the USB interface
      processCommand();   //And to use termite with it      
    }
    
    if(commandAct){
      actonCommand();  //Will be used to find commands and act on them
    }
    
    if((IOT_SELECT)){
      menuSelect=IOT;
    }
    
    if((ADC_SELECT)){
      menuSelect=ADC;
    }
    
    if((BACKLITE)){
      menuSelect=BACKLITE_OP;
    }
    
    if((lineDetected)&&(pause>WAIT_TIME)){
        if(wait>PROTECT_MOTORS){
          RIGHT_FORWARD_SPEED=HALF_SPEED;
          LEFT_REVERSE_SPEED=HALF_SPEED;
        
          if((ADC_Left_Detector>=ADC_Left_Thresh)&&(ADC_Right_Detector>=ADC_Right_Thresh)){
          RIGHT_FORWARD_SPEED=WHEEL_OFF;
          LEFT_REVERSE_SPEED=WHEEL_OFF;
          lineDetected=FALSE;
          pause=FALSE;
          goAuto=TRUE;
          wait=FALSE;
        }
        }
    }
    

    if(goAuto&&(pause>WAIT_TIME)){ //Used to go into Automation function from commands set
      autoAutonomous();
    
    }
    
    if(haltAuto){
      
      switch(haltTime){
        
      case END_CLASS:
        LEFT_FORWARD_SPEED=WHEEL_OFF;
        RIGHT_FORWARD_SPEED=WHEEL_OFF;
        setTime=FALSE;
      strcpy(display_line[INIT], "  BL Stop \0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);        
        break;
        
           
    
             
      default: break;
      }
    }
      
    //==============================================================================    
    
    
    if(menuChoice==FALSE){    //Prints onto the screen which selection is choosen
      menuSelection();
    }

    else{
      menuChoosing();   //Actually goes into either the IOT State machine, or 
    }                   //Configuring the IDE
    
    //=====================================================================
    
    
    
  }
}










