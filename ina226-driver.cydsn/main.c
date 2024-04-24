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
    
    uint16 voltage, current, power;  // the data bois

    for(;;)
    {
        getVoltage(voltage);
        getCurrent(current);
        getPower(power);
        
        Print("Voltage: ");
        PrintInt(voltage);
        Print("\n\r");
        
        Print("Current: ");
        PrintInt(current);
        Print("\n\r");
        
        Print("Power: ");
        PrintInt(power);
        Print("\n\r\n\r");
        
        CyDelay(5000);
        
    }
}

/* [] END OF FILE */
