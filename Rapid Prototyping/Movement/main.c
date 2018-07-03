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

char block0_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L7, H7, 0x00};
char block0_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L15_5, H15_5, 0x00};
char block0_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L4, H4, 0x00};
char block0_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L7, H7, 0x00};
char block0_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L16_5, H16_5, 0x00};
char block0_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

char block2_s6[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x06, L10, H10, 0x00};
char block2_s5[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x05, L16_5, H16_5, 0x00};
char block2_s4[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x04, L5_5, H5_5, 0x00};
char block2_s3[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x03, L7, H7, 0x00};
char block2_s2[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x02, L19_5, H19_5, 0x00};
char block2_s1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,    0x01, L21, H21, 0x00};

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

char test0[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,
                   0x01, L17, H17,
                   0x00};

char test1[] = {HEADER, HEADER, 0x08, 0x03, 0x01, 0xf4, 0x01,
                   0x06, L21, H21,
                   0x00};


void move(char* c);
void pickupBlock(int blockNum);
void moveBlock(int position);
void executeMoveSequence(int blockNum, int position);
void raise(void);

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
      SysCtlDelay(7500000);
      pickupBlock(0);
      SysCtlDelay(7500000);
      raise();
      SysCtlDelay(7500000);
      moveBlock(3);
      SysCtlDelay(7500000);
      raise();
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
            break;
        case 1:
            break;
        case 2:
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
            move(pos4_s4);
            SysCtlDelay(7500000);
            move(pos4_s3);
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
            move(pos5_s4);
            SysCtlDelay(7500000);
            move(pos5_s3);
            SysCtlDelay(7500000);
            move(pos5_s2);
            SysCtlDelay(7500000);
            move(pos5_s1);
            SysCtlDelay(7500000);
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

int robot_won_coin_flip(void){
  srand(time(0));
  return rand() % 2;
}
