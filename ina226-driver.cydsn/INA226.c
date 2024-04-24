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

#include "INA226.h"
#include <math.h>

#define TIMEOUT 20

uint8_t init_INA226()
{
    INA226_I2C_Start();
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
uint8_t reset() {
   return writeReg16(INA226_REG_CONFIG, INA226_RESET);
}

/**
 *   Verify if INA226 is present
 *
 *   Returns:
 *   unsigned char         1 device found
 *                         0 device not found
 */
uint8_t ping() {
    uint16 id;
    uint8 ret = readReg16(INA226_REG_ID, &id);
    if (ret) id = 0;
    return id;
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
uint8_t setShuntResistor(uint8 rShunt)
{    
    return writeReg16(INA226_REG_CALIBRATION, INA226_CALIBRATION_REF / rShunt);
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
uint8 getVoltage(uint16* v)
{
    uint8 res = readReg16(INA226_REG_BUSVOLTAGE, v);
    *v += (*v >> 2); // x1.25, cuz LSB = 1.25mV
    return res;
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
uint8 getShuntVoltage(uint16 *v)
{    
    return readReg16(INA226_REG_SHUNTVOLTAGE, v);
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
uint8 getCurrent(uint16 *c)
{
    return readReg16(INA226_REG_CURRENT, c);
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
uint8 getPower(uint16* p)
{
    return readReg16(INA226_REG_POWER, p);
    // *p = (*p * 3) + (*p >> 3); lol   
}

uint8 readReg16(uint8 reg, uint16* val) {
    uint8 b1, b2;
    INA226_I2C_I2CMasterClearStatus(); //clear the garbage

	INA226_I2C_I2CMasterSendStart(DEVICE_ADDR, INA226_I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	INA226_I2C_I2CMasterWriteByte(reg, TIMEOUT);
	INA226_I2C_I2CMasterSendStop(TIMEOUT);
	
	INA226_I2C_I2CMasterSendStart(DEVICE_ADDR, INA226_I2C_I2C_READ_XFER_MODE, TIMEOUT);
	INA226_I2C_I2CMasterReadByte(INA226_I2C_I2C_ACK_DATA, &b2, TIMEOUT);
    INA226_I2C_I2CMasterReadByte(INA226_I2C_I2C_NAK_DATA, &b1, TIMEOUT);
    
    int err = INA226_I2C_I2CMasterSendStop(TIMEOUT);
    *val = ((uint16) b2 << 8) | b1;
	return err;
}

uint8 writeReg16(uint8 reg, uint16 val) {
    uint8 b1, b2;
    b1 = val & 0xFF;
    b2 = val >> 8;
    INA226_I2C_I2CMasterClearStatus(); //clear the garbage
    
    INA226_I2C_I2CMasterSendStart(DEVICE_ADDR, INA226_I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	INA226_I2C_I2CMasterWriteByte(reg, TIMEOUT);
    
    INA226_I2C_I2CMasterWriteByte(b2, TIMEOUT);
    INA226_I2C_I2CMasterWriteByte(b1, TIMEOUT);
    
    return INA226_I2C_I2CMasterSendStop(TIMEOUT);
}
