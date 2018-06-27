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

uint32_t ui32Value;
int main(void)
{
    //UARTprintf("hello");
    //uint32_t ui32Value;
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

       GPIOPinConfigure(GPIO_PA0_U0RX);
       GPIOPinConfigure(GPIO_PA1_U0TX);
       GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
       UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

           UARTStdioConfig(0, 9600, 16000000);
    // Pin F4 setup
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enable port F
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // Init PF4 as input
        GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,
            GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF4

    // Interrupt setup
      GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);        // Disable interrupt for PF4 (in case it was enabled)
      GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);      // Clear pending interrupts for PF4
      GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown);     // Register our handler function for port F
      GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,
          GPIO_FALLING_EDGE);             // Configure PF4 for falling edge trigger
      GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);     // Enable interrupt for PF4


    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {

    }
    ADCSequenceConfigure(ADC0_BASE,0, ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
                             ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceEnable(ADC0_BASE,0);
    ADCProcessorTrigger(ADC0_BASE,0);
    while(!ADCIntStatus(ADC0_BASE,0, false));
    {

    }
    ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
    //UARTprintf("hallo");
    while(1)
    {

    }
    return 0;



}

void TrigSensor()
{   ADCProcessorTrigger(ADC0_BASE,0);
    while(!ADCIntStatus(ADC0_BASE,0, false));
        {

        }
        ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
        float FsrVolt=ui32Value*5/1023.0;
        float FsrResist=3270.0*(5/FsrVolt-1.0);
        float FsrG=1.0/FsrResist;
        float force;
        if(FsrResist<=600)
        {
            force=(FsrG-0.00075)/0.00000032639;
        }
        else
        {
            force=FsrG/0.000000642857;
        }
        float test = 65.0004;
        UARTprintf("Resistance: %d ohms \n",(int)FsrResist);
        UARTprintf("Resistance: %d g\n",(int)force);

}

void onButtonDown(void) {
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) {
        // PF4 was interrupt cause
        UARTprintf("Button Down\n");
        TrigSensor();
        GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown);   // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,
            GPIO_RISING_EDGE);          // Configure PF4 for rising edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);  // Clear interrupt flag
    }
}
