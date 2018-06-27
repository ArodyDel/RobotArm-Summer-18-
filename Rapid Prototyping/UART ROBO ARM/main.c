#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    unsigned char a = 0x55;
    unsigned char b = 0x08;
    unsigned char c = 0x03;
    unsigned char d = 0x01;
    unsigned char e = 0xE8;
    unsigned char f = 0xD0;
    unsigned char g = 0x07;

    unsigned char lowT=0xF4;
    unsigned char highT=0x01;
    unsigned char PosLo=0x14;
    unsigned char PosHi=0x05;

    UARTCharPut(UART0_BASE, a);
    UARTCharPut(UART0_BASE, a);
    UARTCharPut(UART0_BASE, b);
    UARTCharPut(UART0_BASE, c);
    UARTCharPut(UART0_BASE, d);
    UARTCharPut(UART0_BASE, lowT);
    UARTCharPut(UART0_BASE, highT);
    UARTCharPut(UART0_BASE, d);
    UARTCharPut(UART0_BASE, PosLo);
    UARTCharPut(UART0_BASE, PosHi);


    while (1)
    {
        if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
    }

}
