// File Name : serialcomm.c 
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

extern char display_line[TOTAL_ROWS][NULL_LOC];
extern char *display[TOTAL_ROWS];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern int menuSelect;
extern int check;
extern int menuChoice;
extern int channel;
/////////
extern int White_Left_Avg;
extern int White_Right_Avg;
extern int Black_Left_Avg;
extern int Black_Right_Avg;

extern  unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Left_Detector;
extern volatile unsigned int ADC_Right_Detector;
extern volatile unsigned int ADC_Left_Thresh;
extern volatile unsigned int ADC_Right_Thresh;
extern int oneMore;
extern int scanTime;
extern char ASCII1[DISPLAY_LENGTH];
extern char oneTime;



////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function below is used in order for the user to identify which menu they would like to break into 
//given the current reading of the potentiometer on the power board.  After selection, this function is 
//never revisited.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void menuSelection(void){

      switch(menuSelect){     //Dont print anything else  
        
      case IOT:
        strcpy(display_line[INIT],"->IOT Start \0");
        strcpy(display_line[NEXT],"ADC Config\0");
        update_string(display_line[NEXT], TOP_MID);
        update_string(display_line[INIT], TOP_LINE);  
        strcpy(display_line[CMON], "BACKLITE  \0");
        update_string(display_line[CMON], MID_LOW);
        strcpy(display_line[FINALLY], "  Josh G. \0");
        update_string(display_line[FINALLY], LOW_LOW);
        Display_Update(FALSE,FALSE,FALSE,FALSE);
        check=FALSE;
        break;
        
      case  ADC: 
	    strcpy(display_line[INIT],"IOT Start\0");
        strcpy(display_line[NEXT],"->ADC Conf\0");
        strcpy(display_line[CMON], "BACKLITE  \0");
        strcpy(display_line[FINALLY], "  Josh G. \0");
        update_string(display_line[FINALLY], LOW_LOW);
        update_string(display_line[NEXT], TOP_MID);
        update_string(display_line[INIT], TOP_LINE); 
        update_string(display_line[CMON], MID_LOW);
        Display_Update(FALSE,FALSE,FALSE,FALSE);
        break;
        
      case BACKLITE_OP:
        strcpy(display_line[INIT],"IOT Start \0");
        strcpy(display_line[NEXT],"ADC Config\0");
        strcpy(display_line[CMON], "->BACKLITE\0");
        update_string(display_line[NEXT], TOP_MID);
        update_string(display_line[INIT], TOP_LINE);  
        update_string(display_line[CMON], MID_LOW);
        Display_Update(FALSE,FALSE,FALSE,FALSE);   
        
        break;
      
      default: 
        
        break;

      }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function below is used to execute menu selections.  The IOT or ADC Menus will be executed after selection.
//The Backlite may be toggled as well.
////////////////////////////////////////////////////////////////////////////////////////////////////////

void menuChoosing(void){

    switch(menuChoice){
      case IOT_MENU:
        IOTstateMachine();
        break;
      
      case ADC_MENU:
        P5OUT|=IR_LED;
//        P5OUT|=CHECK_BAT;
        switch(channel){
//          case ADC_THUMB_CHANNEL:
//            HEXtoBCD(ADC_Thumb);
//            strcpy(display_line[FINALLY], ASCII1);
//            update_string(display_line[FINALLY], LOW_LOW);
//            Display_Update(FALSE,FALSE,FALSE,FALSE);
//            break;
            
          case L_DETECT_CHANNEL:
            HEXtoBCD(ADC_Left_Detector);
            strcpy(display_line[CMON], ASCII1);
            update_string(display_line[CMON], MID_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);      
            break;
            
          case R_DETECT_CHANNEL:
            HEXtoBCD(ADC_Right_Detector);
            strcpy(display_line[NEXT], ASCII1);
            update_string(display_line[NEXT], TOP_MID);
            Display_Update(FALSE,FALSE,FALSE,FALSE);   
            break;
      
          default: break;
      }
        switch(scanTime){
          case WHITE_SCAN_READY:
            strcpy(display_line[FINALLY], "Scan White");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);
            oneTime=TRUE;
            RED_LED_ON;
            break;
      
          case WHITE_SCAN_ONE:
            if(oneTime){
              White_Left_Avg+=ADC_Left_Detector;
              White_Right_Avg+=ADC_Right_Detector;
              oneTime=FALSE;
              oneMore=TRUE;
            }
            strcpy(display_line[FINALLY], "Scanning 1 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_OFF;
            break;
            
            
          case WHITE_SCAN_TWO:
            if(oneMore){
              White_Left_Avg+=ADC_Left_Detector;
              White_Right_Avg+=ADC_Right_Detector;
              oneTime=TRUE;
              oneMore=FALSE;
            }
            strcpy(display_line[FINALLY], "Scanning 2 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_ON;
            break;
            
          case WHITE_SCAN_THREE:
            if(oneTime){
              White_Left_Avg+=ADC_Left_Detector;
              White_Right_Avg+=ADC_Right_Detector;
              oneTime=FALSE;
              oneMore=TRUE;
            }
            strcpy(display_line[FINALLY], "Scanning 3 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_OFF;
            break;
            
            
          case WHITE_SCAN_FOUR:
            if(oneMore){
              White_Left_Avg+=ADC_Left_Detector;
              White_Right_Avg+=ADC_Right_Detector;
              oneTime=TRUE;
              oneMore=FALSE;
            }
            strcpy(display_line[FINALLY], "Scanning 4 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_ON;
            break;
            
          case BLACK_SCAN_READY:
            strcpy(display_line[FINALLY], "Scan Black\0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);
            oneTime=TRUE;
            break;
      
          case BLACK_SCAN_ONE:
            if(oneTime){
              Black_Left_Avg+=ADC_Left_Detector;
              Black_Right_Avg+=ADC_Right_Detector;
              oneTime=FALSE;
              oneMore=TRUE;
            }
            strcpy(display_line[FINALLY], "Scanning 1 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_OFF;
            break;
            
            
          case BLACK_SCAN_TWO:
            if(oneMore){
              Black_Left_Avg+=ADC_Left_Detector;
              Black_Right_Avg+=ADC_Right_Detector;
              oneTime=TRUE;
              oneMore=FALSE;
            }
            strcpy(display_line[FINALLY], "Scanning 2 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_ON;
            break;
            
          case BLACK_SCAN_THREE:
            if(oneTime){
              Black_Left_Avg+=ADC_Left_Detector;
              Black_Right_Avg+=ADC_Right_Detector;
              oneTime=FALSE;
              oneMore=TRUE;
            }
            strcpy(display_line[FINALLY], "Scanning 3 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_OFF;
            break;
            
            
          case BLACK_SCAN_FOUR:
            if(oneMore){
              Black_Left_Avg+=ADC_Left_Detector;
              Black_Right_Avg+=ADC_Right_Detector;
              oneTime=TRUE;
              oneMore=FALSE;
            }
            strcpy(display_line[FINALLY], "Scanning 4 \0");
            update_string(display_line[FINALLY], LOW_LOW);
            Display_Update(FALSE,FALSE,FALSE,FALSE);  
            RED_LED_ON;
            break;
          
        case COMPUTATED:
          ADC_Left_Thresh=Black_Left_Avg+White_Left_Avg;
          ADC_Right_Thresh=Black_Right_Avg+White_Right_Avg;
          ADC_Left_Thresh/=TOTAL_DETECT_AVG;
          ADC_Right_Thresh/=TOTAL_DETECT_AVG;
          
          strcpy(display_line[FINALLY], "COMPUTATED! \0");
          update_string(display_line[FINALLY], LOW_LOW);
          Display_Update(FALSE,FALSE,FALSE,FALSE);  
          RED_LED_OFF;
          
          break;
      
          default: break;
      }
      
      break;
      
    default:
     
      break;
    }
}