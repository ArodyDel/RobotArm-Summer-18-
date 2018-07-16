#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <constants.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


int blockNum = 0;

/* Format of char arrays:
 *
 * {HEADER, HEADER, LENGTH, COMMAND, NUM_SERVOS, TIME_LO, TIME_HI,
 *      SERVO_ID_0, ANGLE_0_LO, ANGLE_0_HI, ..., SERVO_ID_N, ANGLE_N_LO, ANGLE_N_HI, 0x00}
 *
 * LENGTH is Number of Parameters, but NOT including either HEADER byte or the NULL terminator 0x00
 */

char init[] = {HEADER, HEADER, 0x17, 0x03, 0x06, 0xf4, 0x01, 0x01, 0xdc, 0x05, 0x02, 0xdc, 0x05, 0x03, 0xdc, 0x05,
                      0x04, 0xdc, 0x05, 0x05, 0xdc, 0x05, 0x06, 0xdc, 0x05, 0x00};

char raise_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, 0x59, 0x06, 0x00};
char raise_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, 0xdc, 0x05, 0x00};
char raise_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, 0xdc, 0x05, 0x00};
char raise_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, 0xdc, 0x05, 0x00};
char raise_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, 0xdc, 0x05, 0x00};
char raise_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L21, H21, 0x00};

char slideO_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L25, H25, 0x00};
char slideO_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L10, H10, 0x00};
char slideO_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L4, H4, 0x00};
char slideO_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L5, H5, 0x00};
char slideO_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char slideO_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L21, H21, 0x00};

char slideX_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L7, H7, 0x00};
char slideX_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L10, H10, 0x00};
char slideX_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L4, H4, 0x00};
char slideX_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L5, H5, 0x00};
char slideX_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char slideX_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L21, H21, 0x00};

char pos0_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L14_25, H14_25, 0x00};
char pos0_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L22_5, H22_5, 0x00};
char pos0_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L14_5, H14_5, 0x00};
char pos0_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L9_5, H9_5, 0x00};
char pos0_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char pos0_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos1_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L13_5, H13_5, 0x00};
char pos1_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L17, H17, 0x00};
char pos1_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L6, H6, 0x00};
char pos1_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L10, H10, 0x00};
char pos1_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L13, H13, 0x00};
char pos1_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos2_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L12, H12, 0x00};
char pos2_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L15, H15, 0x00};
char pos2_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L4, H4, 0x00};
char pos2_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L8, H8, 0x00};
char pos2_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L11, H11, 0x00};
char pos2_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos3_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L16_25, H16_25, 0x00};
char pos3_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L22, H22, 0x00};
char pos3_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L14, H14, 0x00};
char pos3_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L9, H9, 0x00};
char pos3_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char pos3_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos4_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L16_25, H16_25, 0x00};
char pos4_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L17, H17, 0x00};
char pos4_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L6, H6, 0x00};
char pos4_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L9, H9, 0x00};
char pos4_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char pos4_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos5_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L16_25, H16_25, 0x00};
char pos5_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L15, H15, 0x00};
char pos5_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L4, H4, 0x00};
char pos5_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L7, H7, 0x00};
char pos5_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char pos5_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos6_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L18_25, H18_25, 0x00};
char pos6_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L22_5, H22_5, 0x00};
char pos6_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L14_5, H14_5, 0x00};
char pos6_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L9_5, H9_5, 0x00};
char pos6_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L16, H16, 0x00};
char pos6_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos7_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L19, H19, 0x00};
char pos7_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L17, H17, 0x00};
char pos7_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L6, H6, 0x00};
char pos7_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L10, H10, 0x00};
char pos7_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L15, H15, 0x00};
char pos7_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

char pos8_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L20_5, H20_5, 0x00};
char pos8_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x05, L15, H15, 0x00};
char pos8_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x04, L4, H4, 0x00};
char pos8_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x03, L8, H8, 0x00};
char pos8_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x02, L19, H19, 0x00};
char pos8_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x01, L17, H17, 0x00};

void move(char* c);
void moveBlock(int position);
void raise_after_slide(void);
void test_slides_exhaustively(void);
void get_block_from_slide(int);
void movement_setup(void);
void initialize(void);

/* Movement commands to manipulate its six servos are sent to the robot using UART */
void move(char* c){
    UARTprintf("%s", c);
}

/*  Moves a block from the raise_after_slide position to the appropriate position on the game board.
 *  Parameter position is the location on the game board described below.
 *
 *  Layout of game board:
 *
 *      ----------------------
 *      |      |      |      |
 *      |  0   |  1   |  2   |
 *      |------|------|------|
 *      |      |      |      |
 *      |  3   |  4   |  5   |
 *      |------|------|------|
 *      |      |      |      |
 *      |  6   |  7   |  8   |
 *      ----------------------
 *
 *  Each case below moves six servos one-by-one to position the robot and either pick up (POS0 through POS8) or
 *  place a block (X, O).
 *
 */
void moveBlock(int position){
    switch(position){
        case POS0:
            move(pos0_s6); SysCtlDelay(SHORTDELAY);
            move(pos0_s5); SysCtlDelay(SHORTDELAY);
            move(pos0_s4); SysCtlDelay(SHORTDELAY);
            move(pos0_s3); SysCtlDelay(SHORTDELAY);
            move(pos0_s2); SysCtlDelay(SHORTDELAY);
            move(pos0_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS1:
            move(pos1_s6); SysCtlDelay(SHORTDELAY);
            move(pos1_s5); SysCtlDelay(SHORTDELAY);
            move(pos1_s4); SysCtlDelay(SHORTDELAY);
            move(pos1_s3); SysCtlDelay(SHORTDELAY);
            move(pos1_s2); SysCtlDelay(SHORTDELAY);
            move(pos1_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS2:
            move(pos2_s6); SysCtlDelay(SHORTDELAY);
            move(pos2_s5); SysCtlDelay(SHORTDELAY);
            move(pos2_s3); SysCtlDelay(SHORTDELAY);
            move(pos2_s4); SysCtlDelay(SHORTDELAY);
            move(pos2_s2); SysCtlDelay(SHORTDELAY);
            move(pos2_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS3:
            move(pos3_s6); SysCtlDelay(SHORTDELAY);
            move(pos3_s5); SysCtlDelay(SHORTDELAY);
            move(pos3_s4); SysCtlDelay(SHORTDELAY);
            move(pos3_s3); SysCtlDelay(SHORTDELAY);
            move(pos3_s2); SysCtlDelay(SHORTDELAY);
            move(pos3_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS4:
            move(pos4_s6); SysCtlDelay(SHORTDELAY);
            move(pos4_s5); SysCtlDelay(SHORTDELAY);
            move(pos4_s3); SysCtlDelay(SHORTDELAY);
            move(pos4_s4); SysCtlDelay(SHORTDELAY);
            move(pos4_s2); SysCtlDelay(SHORTDELAY);
            move(pos4_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS5:
            move(pos5_s6); SysCtlDelay(SHORTDELAY);
            move(pos5_s5); SysCtlDelay(SHORTDELAY);
            move(pos5_s3); SysCtlDelay(SHORTDELAY);
            move(pos5_s4); SysCtlDelay(SHORTDELAY);
            move(pos5_s2); SysCtlDelay(SHORTDELAY);
            move(pos5_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS6:
            move(pos6_s6); SysCtlDelay(SHORTDELAY);
            move(pos6_s5); SysCtlDelay(SHORTDELAY);
            move(pos6_s4); SysCtlDelay(SHORTDELAY);
            move(pos6_s3); SysCtlDelay(SHORTDELAY);
            move(pos6_s2); SysCtlDelay(SHORTDELAY);
            move(pos6_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS7:
            move(pos7_s6); SysCtlDelay(SHORTDELAY);
            move(pos7_s5); SysCtlDelay(SHORTDELAY);
            move(pos7_s4); SysCtlDelay(SHORTDELAY);
            move(pos7_s3); SysCtlDelay(SHORTDELAY);
            move(pos7_s2); SysCtlDelay(SHORTDELAY);
            move(pos7_s1); SysCtlDelay(SHORTDELAY);
            break;
        case POS8:
            move(pos8_s6); SysCtlDelay(SHORTDELAY);
            move(pos8_s5); SysCtlDelay(SHORTDELAY);
            move(pos8_s3); SysCtlDelay(SHORTDELAY);
            move(pos8_s4); SysCtlDelay(SHORTDELAY);
            move(pos8_s2); SysCtlDelay(SHORTDELAY);
            move(pos8_s1); SysCtlDelay(SHORTDELAY);
            break;
        case X:
            move(slideX_s6); SysCtlDelay(SHORTDELAY);
            move(slideX_s5); SysCtlDelay(SHORTDELAY);
            move(slideX_s4); SysCtlDelay(SHORTDELAY);
            move(slideX_s3); SysCtlDelay(SHORTDELAY);
            move(slideX_s2); SysCtlDelay(SHORTDELAY);
            move(slideX_s1); SysCtlDelay(SHORTDELAY);
            break;
        case O:
            move(slideO_s6); SysCtlDelay(SHORTDELAY);
            move(slideO_s5); SysCtlDelay(SHORTDELAY);
            move(slideO_s4); SysCtlDelay(SHORTDELAY);
            move(slideO_s3); SysCtlDelay(SHORTDELAY);
            move(slideO_s2); SysCtlDelay(SHORTDELAY);
            move(slideO_s1); SysCtlDelay(SHORTDELAY);
            break;
        default:
            break;
    }
}

void raise_after_slide(void){
    move(raise_s3); SysCtlDelay(SHORTDELAY);
    move(raise_s5); SysCtlDelay(SHORTDELAY);
    move(raise_s4); SysCtlDelay(SHORTDELAY);
    move(raise_s2); SysCtlDelay(SHORTDELAY);
    move(raise_s1); SysCtlDelay(SHORTDELAY);
    move(raise_s6); SysCtlDelay(SHORTDELAY);
}

void get_block_from_slide(int side){
    move(init); SysCtlDelay(MEDDELAY);
    moveBlock(side); SysCtlDelay(MEDDELAY);
    char move_0[] = {HEADER, HEADER, 0x0B, 0x03, 0x02, 0x5, 0x5, 0x03, L4, L4, 0x05, L8, H8, 0x00};
    move(move_0); SysCtlDelay(LONGDELAY);
    char move_1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01, 0x06, L15, H15, 0x00};
    move(move_1); SysCtlDelay(MEDDELAY);
    raise_after_slide(); SysCtlDelay(MEDDELAY);
}

void test_slides_exhaustively(void){
    int i, side;
    for(i = 0; i < 9; i++){
        if(i % 2 == 0) { side = X; }
        else{ side = O; }
        get_block_from_slide(side);
        moveBlock(i);
    }
    move(init);
}

void movement_setup(void){
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 9600, 16000000);
}

void initialize(void){
    move(init);
}


