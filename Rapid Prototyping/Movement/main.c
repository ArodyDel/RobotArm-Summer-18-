#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <constants.h>
#include <time.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


int blockNum = 0;
int robot_x = 1;

// format of char arrays:
// {HEADER, HEADER, LENGTH, COMMAND, NUM_SERVOS, TIME_LO, TIME_HI,
//      SERVO_ID_0, ANGLE_0_LO, ANGLE_0_HI, ..., SERVO_ID_N, ANGLE_N_LO, ANGLE_N_HI, 0x00}
//
// LENGTH is Number of Parameters, but NOT including either HEADER byte or the NULL terminator 0x00

char init[] = {HEADER, HEADER, 0x17, 0x03, 0x06, 0xf4, 0x01,
                      0x01, 0xdc, 0x05,
                      0x02, 0xdc, 0x05,
                      0x03, 0xdc, 0x05,
                      0x04, 0xdc, 0x05,
                      0x05, 0xdc, 0x05,
                      0x06, 0xdc, 0x05,
                      0x00};

char raise_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, 0x59, 0x06, 0x00};
char raise_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, 0xdc, 0x05, 0x00};
char raise_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, 0xdc, 0x05, 0x00};
char raise_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, 0xdc, 0x05, 0x00};
char raise_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, 0xdc, 0x05, 0x00};
char raise_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

char chuteX_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L7, H7, 0x00};
char chuteX_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L10, H10, 0x00};
char chuteX_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char chuteX_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L5, H5, 0x00};
char chuteX_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L15, H15, 0x00};
char chuteX_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

char block0_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L7, H7, 0x00};
char block0_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L15_5, H15_5, 0x00};
char block0_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char block0_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L7, H7, 0x00};
char block0_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L16_5, H16_5, 0x00};
char block0_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

char block1_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L25, H25, 0x00};
char block1_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L15_5, H15_5, 0x00};
char block1_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char block1_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L7, H7, 0x00};
char block1_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L13, H13, 0x00};
char block1_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

char block2_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L10, H10, 0x00};
char block2_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L16_5, H16_5, 0x00};
char block2_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L5_5, H5_5, 0x00};
char block2_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L7, H7, 0x00};
char block2_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L19_5, H19_5, 0x00};
char block2_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

char pos0_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L14_25, H14_25, 0x00};
char pos0_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L22_5, H22_5, 0x00};
char pos0_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L14_5, H14_5, 0x00};
char pos0_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L9_5, H9_5, 0x00};
char pos0_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L15, H15, 0x00};
char pos0_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos1_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L13_5, H13_5, 0x00};
char pos1_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L17, H17, 0x00};
char pos1_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L6, H6, 0x00};
char pos1_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L10, H10, 0x00};
char pos1_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L13, H13, 0x00};
char pos1_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos2_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L12, H12, 0x00};
char pos2_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L15, H15, 0x00};
char pos2_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char pos2_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L8, H8, 0x00};
char pos2_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L11, H11, 0x00};
char pos2_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos3_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L16_25, H16_25, 0x00};
char pos3_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L22, H22, 0x00};
char pos3_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L14, H14, 0x00};
char pos3_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L9, H9, 0x00};
char pos3_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L15, H15, 0x00};
char pos3_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos4_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L16_25, H16_25, 0x00};
char pos4_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L17, H17, 0x00};
char pos4_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L6, H6, 0x00};
char pos4_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L9, H9, 0x00};
char pos4_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L15, H15, 0x00};
char pos4_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos5_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L16_25, H16_25, 0x00};
char pos5_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L15, H15, 0x00};
char pos5_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char pos5_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L7, H7, 0x00};
char pos5_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L15, H15, 0x00};
char pos5_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos6_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L18_25, H18_25, 0x00};
char pos6_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L22_5, H22_5, 0x00};
char pos6_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L14_5, H14_5, 0x00};
char pos6_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L9_5, H9_5, 0x00};
char pos6_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L16, H16, 0x00};
char pos6_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos7_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L19, H19, 0x00};
char pos7_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L17, H17, 0x00};
char pos7_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L6, H6, 0x00};
char pos7_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L10, H10, 0x00};
char pos7_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L15, H15, 0x00};
char pos7_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};

char pos8_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L20_5, H20_5, 0x00};
char pos8_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L15, H15, 0x00};
char pos8_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char pos8_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L8, H8, 0x00};
char pos8_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L19, H19, 0x00};
char pos8_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L17, H17, 0x00};


char r_blk1_0[] = {HEADER, HEADER, 0x0e, 0x03, 0x03, 0xf4, 0x01,
                   0x04, 0xbc, 0x02,
                   0x05, 0x08, 0x07,
                   0x06, 0xb6, 0x03,
                   0x00};

char r_blk1_1[] = {HEADER, HEADER, 0x0b, 0x03, 0x02, 0xf4, 0x01,
                   0x02, 0x84, 0x03,
                   0x03, 0xbc, 0x02,
                   0x00};

char r_blk1_2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,
                   0x01, 0x66, 0x08,
                   0x00};

char test0[] = {HEADER, HEADER, 0x0B, 0x03, 0x02, L15, H15,
                   0x03, L5, H5,
                   0x05, L8, H8,
                   0x00};

char test1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,
                   0x06, L15, H15,
                   0x00};


void move(char* c);
void pickupBlock(int blockNum);
void moveBlock(int position);
void executeMoveSequence(int blockNum, int position);
void raise(void);
void raise_after_chute(void);

int robot_won_coin_flip(void);


int main(void) {

    // setup code
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 9600, 16000000);

    move(init);
    SysCtlDelay(15000000);
    int i;
   for(i = 8; i < 9; i++){

       SysCtlDelay(15000000);
       moveBlock(9);
       SysCtlDelay(15000000);
       char move_0[] = {HEADER, HEADER, 0x0B, 0x03, 0x02, 0x5, 0x5,
                          0x03, L4, L4,
                          0x05, L8, H8,
                          0x00};
       move(move_0);
       SysCtlDelay(25000000);
       char move_1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,
                          0x06, L15, H15,
                          0x00};

       move(move_1);
       SysCtlDelay(15000000);
//       char move_2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,
//                             0x01, L17, H17,
//                             0x00};
//       move(move_2);
       raise_after_chute();
       SysCtlDelay(15000000);
       moveBlock(i);
       SysCtlDelay(15000000);
       move(init);
   }


   //move(test1);

//    move(test0);

        //UARTprintf("%d\n", robot_won_coin_flip());

}

void move(char* c){
    UARTprintf("%s", c);
}

void pickupBlock(int blockNum){
    switch(blockNum){
        case 0:
            move(block0_s6);
            SysCtlDelay(7500000);
            move(block0_s5);
            SysCtlDelay(7500000);
            move(block0_s4);
            SysCtlDelay(7500000);
            move(block0_s3);
            SysCtlDelay(7500000);
            move(block0_s2);
            SysCtlDelay(7500000);
            move(block0_s1);
            SysCtlDelay(7500000);
            break;
        case 1:
            move(block1_s6);
            SysCtlDelay(7500000);
            move(block1_s5);
            SysCtlDelay(7500000);
            move(block1_s4);
            SysCtlDelay(7500000);
            move(block1_s3);
            SysCtlDelay(7500000);
            move(block1_s2);
            SysCtlDelay(7500000);
            move(block1_s1);
            SysCtlDelay(7500000);
            break;
        case 2:
            move(block2_s6);
            SysCtlDelay(7500000);
            move(block2_s5);
            SysCtlDelay(7500000);
            move(block2_s4);
            SysCtlDelay(7500000);
            move(block2_s3);
            SysCtlDelay(7500000);
            move(block2_s2);
            SysCtlDelay(7500000);
            move(block2_s1);
            SysCtlDelay(7500000);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        default:
            break;
    }
}

void moveBlock(int position){
    switch(position){
        case 0:
            move(pos0_s6);
            SysCtlDelay(7500000);
            move(pos0_s5);
            SysCtlDelay(7500000);
            move(pos0_s4);
            SysCtlDelay(7500000);
            move(pos0_s3);
            SysCtlDelay(7500000);
            move(pos0_s2);
            SysCtlDelay(7500000);
            move(pos0_s1);
            SysCtlDelay(7500000);
            break;
        case 1:
            move(pos1_s6);
            SysCtlDelay(7500000);
            move(pos1_s5);
            SysCtlDelay(7500000);
            move(pos1_s4);
            SysCtlDelay(7500000);
            move(pos1_s3);
            SysCtlDelay(7500000);
            move(pos1_s2);
            SysCtlDelay(7500000);
            move(pos1_s1);
            SysCtlDelay(7500000);
            break;
        case 2:
            move(pos2_s6);
            SysCtlDelay(7500000);
            move(pos2_s5);
            SysCtlDelay(7500000);
            move(pos2_s3);
            SysCtlDelay(7500000);
            move(pos2_s4);
            SysCtlDelay(7500000);
            move(pos2_s2);
            SysCtlDelay(7500000);
            move(pos2_s1);
            SysCtlDelay(7500000);
            break;
        case 3:
            move(pos3_s6);
            SysCtlDelay(7500000);
            move(pos3_s5);
            SysCtlDelay(7500000);
            move(pos3_s4);
            SysCtlDelay(7500000);
            move(pos3_s3);
            SysCtlDelay(7500000);
            move(pos3_s2);
            SysCtlDelay(7500000);
            move(pos3_s1);
            SysCtlDelay(7500000);
            break;
        case 4:
            move(pos4_s6);
            SysCtlDelay(7500000);
            move(pos4_s5);
            SysCtlDelay(7500000);
            move(pos4_s3);
            SysCtlDelay(7500000);
            move(pos4_s4);
            SysCtlDelay(7500000);
            move(pos4_s2);
            SysCtlDelay(7500000);
            move(pos4_s1);
            SysCtlDelay(7500000);
            break;
        case 5:
            move(pos5_s6);
            SysCtlDelay(7500000);
            move(pos5_s5);
            SysCtlDelay(7500000);
            move(pos5_s3);
            SysCtlDelay(7500000);
            move(pos5_s4);
            SysCtlDelay(7500000);
            move(pos5_s2);
            SysCtlDelay(7500000);
            move(pos5_s1);
            SysCtlDelay(7500000);
            break;
        case 6:
            move(pos6_s6); SysCtlDelay(7500000);
            move(pos6_s5); SysCtlDelay(7500000);
            move(pos6_s4); SysCtlDelay(7500000);
            move(pos6_s3); SysCtlDelay(7500000);
            move(pos6_s2); SysCtlDelay(7500000);
            move(pos6_s1); SysCtlDelay(7500000);
            break;
        case 7:
            move(pos7_s6); SysCtlDelay(7500000);
            move(pos7_s5); SysCtlDelay(7500000);
            move(pos7_s4); SysCtlDelay(7500000);
            move(pos7_s3); SysCtlDelay(7500000);
            move(pos7_s2); SysCtlDelay(7500000);
            move(pos7_s1); SysCtlDelay(7500000);
            break;
        case 8:
            move(pos8_s6);
            SysCtlDelay(7500000);
            move(pos8_s5);
            SysCtlDelay(7500000);
            move(pos8_s3);
            SysCtlDelay(7500000);
            move(pos8_s4);
            SysCtlDelay(7500000);
            move(pos8_s2);
            SysCtlDelay(7500000);
            move(pos8_s1);
            SysCtlDelay(7500000);
            break;
        case 9:
            move(chuteX_s6); SysCtlDelay(7500000);
            move(chuteX_s5); SysCtlDelay(7500000);
            move(chuteX_s4); SysCtlDelay(7500000);
            move(chuteX_s3); SysCtlDelay(7500000);
            move(chuteX_s2); SysCtlDelay(7500000);
            move(chuteX_s1); SysCtlDelay(7500000);
        default:
            break;
    }
}

void executeMoveSequence(int blockNum, int position){
    pickupBlock(blockNum);
    _delay_cycles(240000000);
    raise();
    _delay_cycles(160000000);
    moveBlock(position);
}

void raise(void){
    move(raise_s5);
    SysCtlDelay(7500000);
    move(raise_s4);
    SysCtlDelay(7500000);
    move(raise_s3);
    SysCtlDelay(7500000);
    move(raise_s2);
    SysCtlDelay(7500000);
    move(raise_s1);
    SysCtlDelay(7500000);
    move(raise_s6);
    SysCtlDelay(7500000);
}

void raise_after_chute(void){
    move(raise_s3);
    SysCtlDelay(7500000);
    move(raise_s5);
    SysCtlDelay(7500000);
    move(raise_s4);
    SysCtlDelay(7500000);
    move(raise_s2);
    SysCtlDelay(7500000);
    move(raise_s1);
    SysCtlDelay(7500000);
    move(raise_s6);
    SysCtlDelay(7500000);

}


void test_pos1_fullBoard(void){
    move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(0);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
          move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(1);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(2);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(3);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(4);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(5);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(6);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(7);
          SysCtlDelay(7500000);
          raise();

          SysCtlDelay(7500000);
                move(init);
          SysCtlDelay(7500000);
          pickupBlock(1);
          SysCtlDelay(7500000);
          raise();
          SysCtlDelay(7500000);
          moveBlock(8);
          SysCtlDelay(7500000);
          raise();
}

int robot_won_coin_flip(void){
  srand(time(0));
  return rand() % 2;
}
