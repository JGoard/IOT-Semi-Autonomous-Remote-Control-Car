/ File Name : serialcomm.c 
// Description: functions to pass FRAM Commands or Wifi Commands through
// 
// Author: Josh Goard 
// Date: September 2019 
// Compiler: Built with IAR Embedded Workbench Version: MSP430 (7.12.4) 
//===========================================================================
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include <string.h>

//===============================
//Global Variables
extern char display_line[TOTAL_ROWS][NULL_LOC];
extern char *display[TOTAL_ROWS];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern int setTime;
int interceptTime;
//Analog2Digi
volatile unsigned int ADC_Left_Detector;
volatile unsigned int ADC_Right_Detector;
char lineDetected;

volatile unsigned int ADC_Left_Thresh;
volatile unsigned int ADC_Right_Thresh;

extern char secondlineDetected;


char arrivalString[SHIFT_8][LENGTH]={
  "Arrived 01\0",
  "Arrived 02\0",
  "Arrived 03\0",
  "Arrived 04\0",
  "Arrived 05\0",
  "Arrived 06\0",
  "Arrived 07\0",
  "Arrived 08\0",
};


char goAuto;
extern char haltAuto;

extern int oneTime;

volatile unsigned extern int check=FALSE;
char info[]="AT+NSTAT=?\r";
char port[]="AT+NSTCP=6969,1\r";
char commandbuff[FULL_SEC][MAX_BUFF];
int Buff_Length=FALSE;
extern int index1;
extern char USB_Char_Rx_A0[];
extern int rw;
extern char copyCommand;
extern int pointer;
extern int usb_rx_ring_wr_A0;
char enableCommand=FALSE;\
  char currentCommand[CMD_LENG];
  int currentString=FALSE;
  int oneotherTime=FALSE;
  extern int index2;
  int numstrings=FALSE;
  int morestrings=FALSE;
  int stringCounter=FALSE;
  
  char firstString=TRUE;
  
  char holding;
  extern int commandAct;
  
  extern char IRPause;
  extern int pause;
  extern int wait;
  extern char leftFlag;
  extern char rightFlag;
  
  extern int commandTime;
  
  
  void InitWifi(void){ //Used to find the IP address and SSID of the PSK Network
    int center=FALSE;
    int length=strlen(info);
    
    while(center<length){
      out_character(info[center]);
      center++;
    }
    
  }
  //=========
  
  //=========
  void InitTCPPort(void){
    
    int center=FALSE;
    int length=strlen(port);
    
    while(center<length){
      out_character(port[center]);
      center++;
    }
    
  }
  
  //===============
  
  
  //===============
  
  void processCommand(void){
    
    
    while((rw!=usb_rx_ring_wr_A0)){ //This whole while loop is jsut for a test
      
      if(USB_Char_Rx_A0[rw]=='^'){
        morestrings++;//if seeing carrot on 2nd string, then another command is coming in
        numstrings++;
        index1=FALSE;
      }
      
      if((USB_Char_Rx_A0[rw]!='\r')){
        commandbuff[numstrings][index1]=USB_Char_Rx_A0[rw];//reappropriating numbers n such
        index1++;
      }
      
      if((USB_Char_Rx_A0[rw]=='\r')&&(copyCommand==TRUE)){//if a carriage return, clear everything 
        index1=FALSE;
        index2=FALSE;
        numstrings=FALSE;
        copyCommand=FALSE;
        commandAct=TRUE;
        oneotherTime=TRUE;
        
      }
      
      else rw++;
      
      if(rw>=SMALL_RING_SIZE){
        rw=FALSE;
      }
      
    }
    
  }
  
  
  //==================================
  
  
  //==================================
  
  
  void actonCommand(void){
    if(oneotherTime==TRUE){
      
      
      for (int i=THOUSAND; i<=ONE; i++){
        if(i==THOUSAND){
          commandTime+=((commandbuff[numstrings][i]-ASCII_OFF)*THOUSANDTHS);
        }
        if(i==HUNDREDS){
          commandTime+=((commandbuff[numstrings][i]-ASCII_OFF)*HUNDRETHS);
        }
        if(i==TENS){
          commandTime+=((commandbuff[numstrings][i]-ASCII_OFF)*TENTHS);
        }
        if(i==ONE){
          commandTime+=((commandbuff[numstrings][i]-ASCII_OFF)*ONES);
        }
      }
      commandTime/=INCREMENT;//timer runs every 50ms
      
    }
    
    
    oneotherTime=FALSE;
    
    
    
    switch (commandbuff[numstrings][TRUE]){
      
    case 'F':
      strcpy(display_line[INIT], "  FORWARD \0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      
      
      RIGHT_FORWARD_SPEED=HALF_SPEED;
      LEFT_FORWARD_SPEED=HALF_SPEED;
      endCommand();
      
      break;
      
    case 'R':
      
      strcpy(display_line[INIT], "  REVERSE \0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      
      
      RIGHT_REVERSE_SPEED=HALF_SPEED;
      LEFT_REVERSE_SPEED=HALF_SPEED;
      endCommand();
      
      break;
      
    case 'C':
      
      strcpy(display_line[INIT], "CLOCKWISE!\0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      RIGHT_FORWARD_SPEED=HALF_SPEED;
      LEFT_REVERSE_SPEED=HALF_SPEED;
      setTime=TRUE;
      endCommand();
      
      break; 
      
      
    case 'W':
      
      strcpy(display_line[INIT], "COUNTCLOC!\0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      RIGHT_REVERSE_SPEED=HALF_SPEED;
      LEFT_FORWARD_SPEED=HALF_SPEED;
      endCommand(); 
     
      break; 
      
    case 'I'://Case where if arrived at a square number
      //Will increment if directed too by user
      
      strcpy(display_line[INIT], arrivalString[stringCounter]);
      update_string(display_line [INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      
      if(commandTime==FALSE){
        stringCounter++;
        commandAct=FALSE;
        if(morestrings==FALSE) numstrings=FALSE;
        check=IOT_IN_CHECK;
      }
      
      break;
      
    case 'A':
      
      
      goAuto=TRUE;
      
    case 'L':
      
      IRPause=TRUE;
      strcpy(display_line[INIT], "  BL Start \0");
      update_string(display_line [INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      
      RIGHT_FORWARD_SPEED=QUART_SPEED;
      LEFT_FORWARD_SPEED=HALF_SPEED;
      
      
      
      if(lineDetected==FALSE){
        if(commandTime==FALSE){
          commandTime=MAXTIME;
          if(morestrings==FALSE) numstrings=FALSE;
          check=IOT_IN_CHECK;
        }
        if((morestrings)&&(commandTime==FALSE)){
          morestrings--;
          numstrings++;
          oneotherTime=TRUE;
          commandAct=TRUE;
        }
      }
      if(((ADC_Left_Detector>=ADC_Left_Thresh)&&(ADC_Right_Detector>=ADC_Right_Thresh))&&(pause>WHITE_SCAN_ONE)){//If detected in the loop break from case
        strcpy(display_line[INIT], " Intercept\0");
        pause=FALSE;
        update_string(display_line[INIT], TOP_LINE);
        Display_Update(FALSE,FALSE,FALSE,FALSE);
        RIGHT_FORWARD_SPEED=WHEEL_OFF;                                                         //and start searching for line in main
        LEFT_FORWARD_SPEED=WHEEL_OFF;
        lineDetected=TRUE;
        commandTime=FALSE;
        commandAct=FALSE;
        
      }
      
      break;
      
    case 'S':
      
      LEFT_FORWARD_SPEED=WHEEL_OFF;
      RIGHT_FORWARD_SPEED=WHEEL_OFF;
      LEFT_REVERSE_SPEED=WHEEL_OFF;
      RIGHT_REVERSE_SPEED=WHEEL_OFF;
      check=IOT_IN_CHECK;
      commandAct=FALSE;
      
      break;
      
    case 'P':
      
      strcpy(display_line[INIT], "Loop it!\0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      RIGHT_FORWARD_SPEED=HALF_SPEED;
      LEFT_FORWARD_SPEED=QUART_SPEED;
      
      if(secondlineDetected==FALSE){
        if(commandTime==FALSE){
          commandTime=MAXTIME;
          if(morestrings==FALSE) numstrings=FALSE;
          check=IOT_IN_CHECK;
        }
        if((morestrings)&&(commandTime==FALSE)){
          morestrings--;
          numstrings++;
          oneotherTime=TRUE;
          commandAct=TRUE;
        }
      }
      if((ADC_Left_Detector>=ADC_Left_Thresh)&&(ADC_Right_Detector>=ADC_Right_Thresh)){//If detected in the loop break from case
        RIGHT_FORWARD_SPEED=WHEEL_OFF;                                                         //and start searching for line in main
        LEFT_FORWARD_SPEED=WHEEL_OFF;
        secondlineDetected=TRUE;
        commandTime=FALSE;
        commandAct=FALSE;
        
      }
      
      break;
      
    case END_IT:       
      goAuto=FALSE;
      haltAuto=TRUE;
      strcpy(display_line[INIT], "  BL Exit \0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      
      RIGHT_FORWARD_SPEED=HALF_SPEED;
      LEFT_FORWARD_SPEED=HALF_SPEED;
      commandAct=FALSE;// beta
      
      break;
      
    case 'T':
      
      strcpy(display_line[INIT], "Slite Rite\0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      RIGHT_FORWARD_SPEED=QUART_SPEED;
      LEFT_FORWARD_SPEED=HALF_SPEED;
      endCommand();
            
      break;
      
      
    case 'O':
      
      strcpy(display_line[INIT], "Slite Left\0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      RIGHT_FORWARD_SPEED=HALF_SPEED;
      LEFT_FORWARD_SPEED=QUART_SPEED;
      endCommand();
     
      break;
      
      
      
    default: break;
    
    }
    
  }
  
  void IOTstateMachine(void){
    
    switch(check){ //Time based statemachine that runs through startup and will reset if 
      //left inactive
      
    case RESET:
      RED_LED_OFF;
      GRN_LED_OFF;
      break;
      
    case IOT_INIT: //Initial Startup initializing
      //
      P5OUT |= IOT_RESET;
      oneTime=TRUE;
      RED_LED_ON;
      GRN_LED_OFF;
      break;
      
    case PRINT_IP: //5 seconds probe for information on SSID and IP Add.
      if(oneTime==TRUE){
        InitWifi();
        oneTime=FALSE;
      }
      break;
      
    case PRINT_SSID:
      InitSSID();
      break;
      
    case SET: //Will move too fast if not used and the buffer will be overfilled
      oneTime=TRUE;
      break;
      
    case TCP_INIT:
      if(oneTime==TRUE){
        InitTCPPort(); //Send a command to open a tcp port
        oneTime=FALSE;
      }
      GRN_LED_ON;
      RED_LED_OFF;      
      break;
      
    case RESET_IMINENT_5:
      RED_LED_OFF;
      GRN_LED_OFF;
      break;
      
    case RESET_IMINENT_4:
      RED_LED_ON;
      GRN_LED_ON;
      break;
      
    case RESET_IMINENT_3:
      RED_LED_OFF;
      GRN_LED_OFF;
      break;
      
    case RESET_IMINENT_2:
      RED_LED_ON;
      GRN_LED_ON;
      break;
      
    case RESET_IMINENT_1:
      RED_LED_OFF;
      GRN_LED_OFF;
      break;
      
    case RESET_IMINENT_0:
      P5OUT &= ~IOT_RESET;
      RED_LED_ON;
      GRN_LED_ON;
      break;
      
    case RESET_INIT:
      check=FALSE;
      GRN_LED_OFF;
      RED_LED_OFF;
      break;
      
      
    default: break;
    }
    
    
  }
  
  void autoAutonomous(void){
    
    if(wait>PROTECT_MOTORS){
      check=IOT_IN_CHECK;
      strcpy(display_line[INIT], " BL Travel\0");
      update_string(display_line[INIT], TOP_LINE);
      Display_Update(FALSE,FALSE,FALSE,FALSE);
      
      if((ADC_Left_Detector>=ADC_Left_Thresh)&&(ADC_Right_Detector>=ADC_Right_Thresh)){
        //Follow Straight
        LEFT_FORWARD_SPEED=HALF_SPEED;
        RIGHT_FORWARD_SPEED=HALF_SPEED;
        rightFlag=FALSE;
        leftFlag=FALSE;
        
      }
      else if((ADC_Left_Detector>=ADC_Left_Thresh)&&(ADC_Right_Detector<=ADC_Right_Thresh)){    
        LEFT_FORWARD_SPEED=QUART_SPEED;
        RIGHT_FORWARD_SPEED=HALF_SPEED;
        rightFlag=FALSE;
        leftFlag=TRUE;
      }
      
      
      else if((ADC_Left_Detector<=ADC_Left_Thresh)&&(ADC_Right_Detector>=ADC_Right_Thresh)){
        LEFT_FORWARD_SPEED=HALF_SPEED;
        RIGHT_FORWARD_SPEED=QUART_SPEED;
        rightFlag=TRUE;
        leftFlag=FALSE;
      }
      
      else {
        if(rightFlag==TRUE){
          LEFT_FORWARD_SPEED=HALF_SPEED;
          RIGHT_FORWARD_SPEED=WHEEL_OFF;
        }
        
        if(leftFlag==TRUE){
          LEFT_FORWARD_SPEED=WHEEL_OFF;
          RIGHT_FORWARD_SPEED=HALF_SPEED;
          
        }
        
      }
    }
  
  }
  
  void endCommand(void){
      if(commandTime==FALSE){
        RIGHT_FORWARD_SPEED=WHEEL_OFF;
        LEFT_FORWARD_SPEED=WHEEL_OFF;
        RIGHT_REVERSE_SPEED=WHEEL_OFF;
        LEFT_REVERSE_SPEED=WHEEL_OFF;
        commandAct=FALSE;
        if(morestrings==FALSE) numstrings=FALSE;
        check=IOT_IN_CHECK;
      }
      if((morestrings)&&(commandTime==FALSE)){
        morestrings--;
        numstrings++;
        oneotherTime=TRUE;
        commandAct=TRUE;
      }
  
  
  
  
  
  
  
  }
  
  
  
  
  
  
  