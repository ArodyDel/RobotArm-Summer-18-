#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"

/**
 * main.c
 */

void onButtonDown(void);
unsigned char Mask7seg=0x01;
int count=0;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);



    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 );


    // Pin F4 and F0 setup
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enable port F
            HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;//Disable the device on F0
            HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

            GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 |GPIO_PIN_0);  // Init PF4 and PF0 as input
            GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 |GPIO_PIN_0,
                GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF4

        // Interrupt setup
          GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);        // Disable interrupt for PF4 (in case it was enabled)
          GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);      // Clear pending interrupts for PF4
          GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown);     // Register our handler function for port F
          GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0,
              GPIO_FALLING_EDGE);             // Configure PF4 for falling edge trigger
          GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);

          while(1)
          {

          }

	return 0;
}


void onButtonDown(void) {
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) {       //sw1 button handler
        // PF4 was interrupt cause
        //UARTprintf("Button Down\n");
        //TrigSensor();
                  // Configure PF4 for rising edge trigger

        switch (count)
        {
        case 0:
            Mask7seg=0x3F;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 1:
            Mask7seg=0x06;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 2:
            Mask7seg=0x5B;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 3:
            Mask7seg=0x4F;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 4:
            Mask7seg=0x66;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 5:
            Mask7seg=0x6D;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 6:
            Mask7seg=0x7D;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 7:
            Mask7seg=0x07;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 8:
            Mask7seg=0x7F;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 9:
            Mask7seg=0x67;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        }
        if(count==9)
                {
                    count=0;
                }
                else
                {
                        count++;
                }
        SysCtlDelay(2000000); //delay
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);  // Clear interrupt flag

    }
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0) {       // sw2 interrupt handle

        Mask7seg=0x40;
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                                   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );

        SysCtlDelay(2000000);
                GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);  // Clear interrupt flag
    }
}
