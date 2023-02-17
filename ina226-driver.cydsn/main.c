/* 
    It's a me. Da pogrammer!
*/
#include "main.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    DBG_UART_Start();
    PrintInt(init_INA226());
    
    Print("\r\nINITIALIZING\n\r");
    
    uint8_t voltage[2], current[2], power[2];  // the data bois

    for(;;)
    {
        getVoltage(voltage);
        getCurrent(current);
        getPower(power);
        
        Print("Voltage: ");
        PrintInt((voltage[0] << 8) + voltage[1]);
        Print("\n\r");
        
        Print("Current: ");
        PrintInt((current[0] << 8) + current[1]);
        Print("\n\r");
        
        Print("Power: ");
        PrintInt((power[0] << 8) + power[1]);
        Print("\n\r\n\r");
        
        CyDelay(5000);
        
    }
}

/* [] END OF FILE */
