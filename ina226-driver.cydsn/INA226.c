/*
 * Copyright (c) 2016 by Stefano Speretta <s.speretta@tudelft.nl>
 *
 * INA226: a library to provide high level APIs to interface with the
 * TI INA226 current sensor. It is possible to use this library in
 * Energia (the Arduino port for MSP microcontrollers) or in other
 * toolchains.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * version 3, both as published by the Free Software Foundation.
 *
 */

#include <INA226.h>


uint8_t init_INA226()
{
    I2C_INA226_Start();
    reset();
    setShuntResistor(100);
    return ping();
}

/**
 *   Reset the INA226
 *
 *   Returns:
 *   unsigned char         0 succes
 *                         1 fail
 */
uint8_t reset()
{
   uint8_t data[2];
   data[0] = (INA226_RESET) >> 8;
   data[1] = 0;  //INA226_RESET;
   return writeRegister(INA226_REG_CONFIG, data, 2);
}

/**
 *   Verify if INA226 is present
 *
 *   Returns:
 *   unsigned char         1 device found
 *                         0 device not found
 */
uint8_t ping()
{
    uint8_t id[2];
    readRegister(INA226_REG_ID, id, 2);
    return ((id[0] << 8) + id[1]);
}

/**
 *   Sets the shunt resistor value in mOhm
 *
 *   Parameters:
 *   unsigned short shunt  shunt resistor value in mOhm
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 */
uint8_t setShuntResistor(uint8_t shunt)
{
    uint8_t data[2];
    data[0] = (INA226_CALIBRATION_REF / shunt) >> 8;
    data[1] = INA226_CALIBRATION_REF / shunt;
    return writeRegister(INA226_REG_CALIBRATION, data, 2);
}

uint8_t setAlertLimitBusVoltage(uint8_t limit)
{
    uint8_t data[1];
    data[0] = (uint8_t)(0.8 * limit);
    return writeRegister(INA226_REG_ALERTLIMIT, data, 1);  //translate voltage to whole decimal (/2.5uV)
}

uint8_t setAlertEnableBusUnderVoltage()
{
    uint8_t data[2];
    data[0] = (INA226_BIT_BUL) >> 8;
    data[1] = 0;  //INA226_BIT_BUL;
    return writeRegister(INA226_REG_MASKENABLE, data, 2);
}
/**
 *   Returns the bus voltage in mV
 *
 *   Parameters:
 *   unsigned short &      bus voltage in mV
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 */
uint8_t getVoltage(uint8_t *v)
{
    uint8_t ret = readRegister(INA226_REG_BUSVOLTAGE, v, 2);
    *v = *v + (*v >> 2);
    if (ret)
    {
        *v = 0;
    }
    return ret;
}

/**
 *   Returns the voltage across the shunt resistor
 *
 *   Parameters:
 *   signed short &      bus voltage (LSB = 2.5 uV)
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 */
uint8_t getShuntVoltage(uint8_t *v)
{
    uint8_t ret = readRegister(INA226_REG_SHUNTVOLTAGE, v, 2);
    if (ret)
    {
        *v = 0;
    }
    return ret;
}

/**
 *   Returns the current through the shunt resistor
 *
 *   Parameters:
 *   signed short &        current in mA
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 */
uint8_t getCurrent(uint8_t *c)
{
    uint8_t ret = readRegister(INA226_REG_CURRENT, c, 2);
    *c/=8;
    if (ret)
    {
        *c = 0;
    }
    return ret;
}

/**
 *   Returns the power across the load in mW
 *
 *   Parameters:
 *   unsigned short &      power in mW
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 */
uint8_t getPower(uint8_t *p)
{
    uint8_t ret = readRegister(INA226_REG_POWER, p, 2);
    *p = (*p * 3) + (*p >> 3);
    if (ret)
    {
        *p = 0;
    }
    return ret;
}

// Returns the value of the selected internal register
uint8_t readRegister(uint8_t reg, uint8_t *output, uint8_t cnt)
{
    I2C_INA226_I2CMasterClearStatus(); //clear the garbage

    int ms_timeout = 20;
    uint32_t error = 0; // this is the "status" we usually use in our R/W functions
	uint8_t idx;
	error = I2C_INA226_I2CMasterSendStart(DEVICE_ADDR, I2C_INA226_I2C_WRITE_XFER_MODE, ms_timeout);
	
	error = I2C_INA226_I2CMasterWriteByte(reg, ms_timeout);
	
	error = I2C_INA226_I2CMasterSendStop(ms_timeout);
	
	error = I2C_INA226_I2CMasterSendStart(DEVICE_ADDR, I2C_INA226_I2C_READ_XFER_MODE, ms_timeout);
    //PrintInt(BNO055_iERROR);
	for (idx = 0; (idx < cnt) && (error == 0); idx++)
	{
        if (idx < cnt-1)
        {
		    I2C_INA226_I2CMasterReadByte(I2C_INA226_I2C_ACK_DATA, &output[idx], ms_timeout);
        }
        else
        {
            I2C_INA226_I2CMasterReadByte(I2C_INA226_I2C_NAK_DATA, &output[idx], ms_timeout);
        }
	}
	// Check for BNO055_iERROR before proceeding
	error = I2C_INA226_I2CMasterSendStop(ms_timeout);

	return (uint8_t)error;
}

//Sets the value of the selected internal register
uint8_t writeRegister(uint8_t reg, uint8_t *data, uint8_t cnt)
{
    
    I2C_INA226_I2CMasterClearStatus(); //clear the garbage
    uint8_t data_pack[cnt + 1];
    data_pack[0] = reg;
    data_pack[1] = data[0];
    data_pack[2] = data[1];
    
    int status = I2C_INA226_I2CMasterWriteBuf(DEVICE_ADDR, data_pack, cnt, I2C_INA226_I2C_MODE_COMPLETE_XFER);
    while ((I2C_INA226_I2CMasterStatus() & I2C_INA226_I2C_MSTAT_WR_CMPLT) == 0u) //should wait for write buffer to complete
    {
        Print("\r\nWRITE TO: \n\r");
        PrintInt(reg);
    }
	return status;  
}
