/**
 *
 *   Returns the value of the selected internal register
 *
 *   Parameters:
 *   unsigned char reg     register number
 *   unsigned short &      register value
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */

unsigned char INA226::readRegister(unsigned char reg, unsigned short &output)
{
    wire.beginTransmission(address);
    wire.write(reg);

    unsigned char res = wire.requestFrom(address, 2);
    if (res == 2)
    {
        output = ((unsigned short)wire.read()) << 8;
        output |= wire.read() & 0xFF;
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 *
 *   Sets the value of the selected internal register
 *
 *   Parameters:
 *   unsigned char reg     register number
 *   unsigned short        register value
 *
 *   Returns:
 *   unsigned char         0 success
 *                         1 fail
 *
 */
unsigned char INA226::writeRegister(unsigned char reg, unsigned short val)
{
    wire.beginTransmission(address);
    wire.write(reg);
    wire.write((val >> 8) & 0xFF);
    wire.write(val & 0xFF);
    return wire.endTransmission();
}
