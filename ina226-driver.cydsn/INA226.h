#include "project.h"
#include "stdlib.h"
#include <stdint.h>

#define DEVICE_ADDR                 0x40

#define INA226_DEVICE_ID            (0x2260)
#define INA226_RESET                (0x8000)
#define INA226_CALIBRATION_REF      (0xA000)

#define INA226_REG_CONFIG           (0x00)
#define INA226_REG_SHUNTVOLTAGE     (0x01)
#define INA226_REG_BUSVOLTAGE       (0x02)
#define INA226_REG_POWER            (0x03)
#define INA226_REG_CURRENT          (0x04)
#define INA226_REG_CALIBRATION      (0x05)
#define INA226_REG_MASKENABLE       (0x06)
#define INA226_REG_ALERTLIMIT       (0x07)
#define INA226_REG_ID               (0xFF)

#define INA226_BIT_SOL              (0x8000)
#define INA226_BIT_SUL              (0x4000)
#define INA226_BIT_BOL              (0x2000)
#define INA226_BIT_BUL              (0x1000)
#define INA226_BIT_POL              (0x0800)
#define INA226_BIT_CNVR             (0x0400)
#define INA226_BIT_AFF              (0x0010)
#define INA226_BIT_CVRF             (0x0008)
#define INA226_BIT_OVF              (0x0004)
#define INA226_BIT_APOL             (0x0002)
#define INA226_BIT_LEN              (0x0001)

//Debugging macros
#define Print(message) DBG_UART_UartPutString(message)
#define PrintChar(character) DBG_UART_UartPutChar(character)
#define PrintInt(integer) DBG_UART_UartPutString(itoa(integer, debugOutput, 10))
#define PrintIntBin(integer) DBG_UART_UartPutString(itoa(integer, debugOutput, 2))
#define BlinkDBG() DBG_Write( ! DBG_Read() )

char debugOutput[32];

 
uint8 address;

uint8 reset();
uint8 ping();

uint8 init_INA226();

// configure the device
uint8 setShuntResistor(uint8);
uint8 setAlertLimitBusVoltage(uint8);
uint8 setAlertEnableBusUnderVoltage();


// functions used to retrieve the measurements from the device
uint8 getShuntVoltage(uint16*);
uint8 getVoltage(uint16*);
uint8 getCurrent(uint16*);
uint8 getPower(uint16*);


// only for use
uint8 readReg16(uint8 reg, uint16* val);
uint8 writeReg16(uint8 reg, uint16 val);
uint32 readRegister(uint8 reg, uint8 *output, uint8 cnt);
uint32 writeRegister(uint8_t reg, uint8_t *data, uint8_t cnt);
