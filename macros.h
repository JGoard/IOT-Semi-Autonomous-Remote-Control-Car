//=========================================================================== 
// File Name : macros.h 
// Description: This file contains the #defines in the program for the 
// 
// Author: Josh Goard 
// Date: September 2019 
// Compiler: Built with IAR Embedded Workbench Version: MSP430 (7.12.4) 
//===========================================================================
//ADC Channel
// #define 
// #define
// #define

//ASCII Constants
#define RETURN ('\r')
#define CMD_LENG (15)
#define STRING_MAX (5)

//Time 
#define TB0CCR0_INTERVAL (25000)//50ms
#define TB0CCR1_INTERVAL (2500)//5ms
#define TB0CCR2_INTERVAL (3125)
#define CCR1 (2)
#define CCR2 (4)
#define OVERFLOW(14)
#define MAX_BUFF (6)
#define HEX_NUMBER (0x01ff)
#define THOUSAND (2)
#define HUNDREDS (3)
#define TENS 	(4)
#define ONE (5)
#define EIGHT_MHZ (243)
#define INCREMENT (50)

#define THOUSANDTHS (1000)
#define HUNDRETHS (100)
#define TENTHS (10)
#define ONES (1) 
#define CLEAR (0xFFFF)

//line length
#define LENGTH (11)
#define WORD_LENGTH (5)
#define AVERAGE (2)
#define WAIT (0)
#define TWO_ROWS (2)
//Scanning for Black/White
#define WHITE_SCAN_READY (20)
#define WHITE_SCAN_ONE (60)
#define WHITE_SCAN_TWO (80)
#define WHITE_SCAN_THREE (100)
#define WHITE_SCAN_FOUR (120)
#define WHITE_SCAN_FIVE (140)
#define WHITE_SCAN_COMMENCE (160)
#define BLACK_SCAN_READY (180)
#define BLACK_SCAN_ONE (240)
#define BLACK_SCAN_TWO (260)
#define BLACK_SCAN_THREE (280)
#define BLACK_SCAN_FOUR (300)
#define BLACK_SCAN_FIVE (320)
#define COMPUTATED      (380)
#define EIGHT_BIT(256)

//Motor Speeds (PWM)
#define WHEEL_OFF (0)
#define WHEEL_PERIOD (40000)

#define FULL_SPEED (35000)
#define HALF_SPEED (20000)
#define FAST_TURN (15000)
#define SEVENTH_SPEED (7000)
#define QUART_SPEED (10000)
#define EIGHTH_SPEED (5000)

#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED (TB3CCR4)

#define IOT_IN_CHECK (160)
#define PARSE_PARSE (7)
#define MAXTIME (9999)
//
#define  FIRST (9)
#define  SECOND (99)
#define  THIRD (999)
#define CYCLES (3000)
#define DCOTAP(0x0100)
#define DCOFTRIM (0x0070)

//Line Detector Minimum thresholds
#define LEFT_DETECT (1200) //
#define RIGHT_DETECT (1200) //Left is at 0x50
#define TOTAL_DETECT_AVG (8)
#define ADC_THUMB_CHANNEL (0)
#define L_DETECT_CHANNEL(1)
#define R_DETECT_CHANNEL (2)
#define V_BAT_CHANNEL (3)

//Passcode
#define INITIAL_PASS (10)
#define CONTINUE_PASS (20)
#define CONTINUING_PASS (30)
#define END_PASSWORD (40)


//bit mask
#define MASK (0x0F)
#define SHIFT_2 (2)
#define SHIFT_4 (4)
#define SHIFT_8 (8)
#define ASCII_OFF (0x30)
#define ASCII_OFF_2 (0x37)
#define ERASE (0)

//buttons
#define BUTTON_2 ((!(P2IN&SW2)))
#define BUTTON_1  ((!(P4IN&SW1)))
#define NOTBUTTON1 (P4IN&SW1)
#define NOTBUTTON2 (P2IN&SW2)

#define RED_LED_ON      (P1OUT |= RED_LED)
#define RED_LED_OFF    (P1OUT &= ~RED_LED)
#define GRN_LED_ON    (P6OUT |= GRN_LED)
#define GRN_LED_OFF  (P6OUT &= ~GRN_LED)
#define ALWAYS                         (1)
#define RESET_STATE                    (0)
#define GRN_LED                     (0x40) // GREEN LED 1

//Motors on macros
#define T_R_F     (P6OUT|=R_FORWARD) 
#define T_L_F      (P6OUT|=L_FORWARD) 
#define T_R_R      (P6OUT |= R_REVERSE)
#define T_L_R      (P6OUT |= L_REVERSE) 

#define FULL_SEC (5)
#define BAUD_INIT (4)
#define FAST_BAUD_RATE(0x5551)

#define NEWLINE (0x0A)
#define CARRIAGE (0x0D)
#define CHECK_UART_VECTOR (0x08)
#define CHECK_TIME_VECTOR (14)
#define NO_RX_TX (0)
#define RECEIVE_CASE (2)
#define TRANSFER_CASE (4)
#define SPLIT_SSID (8)
#define PADDING (9)
#define PADDED (8)
#define IP_ROW_1 (2)
#define IP_ROW_2 (8)
#define PARSE_IP (14)

#define FORWARD (F)
#define REVERSE (R)


//IOT RESET
#define RESET           (0)
#define IOT_INIT        (20)
#define PRINT_IP        (100)
#define PRINT_SSID      (101)
#define SET             (102)
#define TCP_INIT        (103)
#define RESET_IMINENT_5 (900)
#define RESET_IMINENT_4 (910)
#define RESET_IMINENT_3 (920)
#define RESET_IMINENT_2 (930)
#define RESET_IMINENT_1 (940)
#define RESET_IMINENT_0 (950)
#define RESET_INIT      (960)
#define WAIT_TIME		(40)
#define PROTECT_MOTORS (10)
#define FRAM_LENGTH		(10)
#define IP_ADD_L1 (0)
#define IP_ADD_L2 (1)
//Strings for Arrival  
#define ARRIVAL1 (100)
#define ARRIVAL2 (200)
#define ARRIVAL3 (300)
#define ARRIVAL4 (400)
#define ARRIVAL5 (500)
#define ARRIVAL6 (600)
#define ARRIVAL7 (700)
#define ARRIVAL8 (800)

//Menu Selection
#define BACKLITE   ((3000<ADC_Thumb)&&(ADC_Thumb<4000))
#define IOT_SELECT ((0<ADC_Thumb)&&(ADC_Thumb<2000))
#define ADC_SELECT ((2000<ADC_Thumb)&&(ADC_Thumb<3000))
#define END_CLASS (100)

//Menu Choice
#define IOT     (1)
#define ADC     (2)
#define BACKLITE_OP (3)
#define IOT_MENU (1)
#define ADC_MENU (2)

#define END_IT (0x1B)

#define COMMAND (4)
#define RESET (0)


#define LCD_LITE_ON        (P6OUT|=LCD_BACKLITE)

#define START_WHEELS    (P2OUT&=~SW2)
#define LITE_ON         (P4OUT|=SW1)
//#define HOLD_MOTORS     (5)
//Main Magic Numbers

#define TOP_LINE       (0) //Used for the sections on the LCD from Top ---> Bottom
#define TOP_MID        (1)
#define MID_LOW        (2)
#define LOW_LOW        (3)
#define TOTAL_ROWS     (4)
#define NULL_LOC     (11)
#define RESET         (0)
#define TOP             (9)
//TRUE OR FALSE


#define START_COM       (0x1B)
#define DISPLAY_LENGTH (11)
//Serial Communication
#define BEGINNING (0)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (32)
#define ADDRESS_SPACE (20)

//

#define TRUE            (1)//Used for checking conditions in main
#define FALSE           (0)
#define SCANNING_WHITE          (1)
#define CALIBRATING_WHITE       (2)
#define SCANNING_BLACK          (3)
#define CALIBRATING_BLACK       (4)


//STRING ARRAY MACROS
#define INIT              (0)
#define NEXT              (1)
#define CMON              (2)
#define FINALLY           (3)
#define START             (4)
#define MAX               (11)


//============================================= 
//Port 1 pins
//Used by init_Port1
//========================================
#define RED_LED         (0x01) // 0 RED LED 0
#define A1_SEEED        (0x02) // 1 A1_SEEED
#define V_DETECT_L      (0x04) // 2 BO_SDA A2
#define V_DETECT_R      (0x08) // 3 BO_SCL A3
#define V_BAT           (0x10) // 4 A4_SEEED
#define V_THUMB         (0x20) // 5 A5
#define UCA0RXD         (0x40) // 6 Back Channel UCA0RXD
#define UCA0TXD         (0x80) // 7 Back Channel UCA0TXD



// Port 2 Pins
#define P2_0            (0x01) // 0 GP I/O
#define P2_1            (0x02) // 1 GP I/0
#define P2_2            (0x04) // 2 GP I/O
#define SW2             (0x08) // 3 SW2 (No Pin on Board.  Built in Switch/Button)
#define P2_4            (0x10) // 4 GP I/O
#define P2_5            (0x20) // 5 GP I/O
#define LFXOUT          (0x40) // XOUT
#define LFXIN           (0x80) // XIN



// Port 3 Pins
#define TEST_PROBE      (0x01) // 0 TEST PROBE GP I/O
#define OA2O            (0x02) // 1 Photodiode Circuit(OPAMP)
#define OA2N            (0x04) // 2 Photodiode Circuit(OPAMP)
#define OA2P            (0x08) // 3 Photodiode Circuit(OPAMP)
#define SMCLK           (0x10) // 4 SMCLK
#define P3_5            (0x20) // 5 GP I/O *****CHECK WITH TA'S/PIAZZA*****
#define IOT_LINK        (0x40) // 6 IOT_LINK
#define P3_7            (0x80) // 7 GP I/O



// Port 4 Pins
#define RESET_LCD       (0x01) // 0 LCD Reset
#define SW1             (0x02) // 1 SW1 GP I/O(?????)
#define UCA1RXD         (0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD         (0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD     (0x10) // 4 Chip Select
#define UCB1SCK         (0x20) // 5 SPI mode clock output UCB1SCK
#define UCB1SIMO        (0x40) // 6 UCB1SIMO
#define UCB1SOMI        (0x80) // 7 UCB1SOMI


// Port 5 Pins 
#define IOT_RESET       (0x01) // 0 IOT_RESET A8 GP I/O
#define IR_LED          (0x02) // 1 IR_LED A9 GP I/O
#define IOT_PROG_SEL    (0x04) // 2 IOT_PROG_SEL A10 GP I/O
#define IOT_PROG_MODE   (0x08) // 3 IOT_PROG_MODE A11 GP I/O
#define CHECK_BAT       (0x10) // 4 CHECK_BAT GP I/O


//Port 6 Pins
#define R_FORWARD       (0x01) // 0 TB 3.1 
#define L_FORWARD       (0x02) // 1 TB 3.2
#define R_REVERSE       (0x04) // 2 TB 3.3
#define L_REVERSE       (0x08) // 3 TB 3.4
#define LCD_BACKLITE    (0x10) // 4 GP I/O
#define P6_5            (0x20) // 5 GP I/O
#define GRN_LED         (0x40) // 6 GP I/O GREEN LED 1
