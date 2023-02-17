#include "project.h"
#include "INA226.h"


#define CHIP_TYPE CHIP_TYPE_PSOC_CY8C4248AZI_L485

//Debugging macros
#define Print(message) DBG_UART_UartPutString(message)
#define PrintChar(character) DBG_UART_UartPutChar(character)
#define PrintInt(integer) DBG_UART_UartPutString(itoa(integer, debugOutput, 10))
#define PrintIntBin(integer) DBG_UART_UartPutString(itoa(integer, debugOutput, 2))
#define BlinkDBG() DBG_Write( ! DBG_Read() )

char debugOutput[32];