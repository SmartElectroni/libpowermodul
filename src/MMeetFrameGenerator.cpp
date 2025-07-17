#include "../include/libmodul.h"

#define CAN_INV_DLC                     8
#define CAN_INV_EFF_FLAG                0x80000000
 
const uint8_t LOW_MODE = 0x00;                
const uint8_t HIGH_MODE = 0x01; 
const uint8_t AUTO_MODE = 0x02;           
const uint8_t OFF = 0x01;                     
const uint8_t ON = 0x00;

can_frame MMeetFrameGenerator::init_frame(uint8_t module_address)
{
    can_frame frame{};
    frame.can_dlc = CAN_INV_DLC;
    frame.can_id = (0x01 << 25 | 0x01 << 21 | (module_address & 0b01111111) << 14 | 0x00 << 9 | 0x00);       
    frame.can_id |= CAN_INV_EFF_FLAG;
    return frame;
}

can_frame MMeetFrameGenerator::generateTempRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x1E;
    return frame;
}

can_frame MMeetFrameGenerator::generateCurrentCapabilityRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x68;
    return frame;
}
    
can_frame MMeetFrameGenerator::generateFlagsRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x08;
    return frame;
}

can_frame MMeetFrameGenerator::generateVoltageRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x62;
    return frame;
}

can_frame MMeetFrameGenerator::generateCurrentRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x30;
    return frame;
}

can_frame MMeetFrameGenerator::generateLowModeSet(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x10;
    frame.data[1] = 0x5F;
    frame.data[7] = LOW_MODE;
    return frame;
}

can_frame MMeetFrameGenerator::generateHighModeSet(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x10;
    frame.data[1] = 0x5F;
    frame.data[7] = HIGH_MODE;
    return frame;
}

can_frame MMeetFrameGenerator::generateAutoModeSet(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x10;
    frame.data[1] = 0x5F;
    frame.data[7] = AUTO_MODE;
    return frame;
}
   
can_frame MMeetFrameGenerator::generateVoltageSet(uint8_t module_address, uint16_t voltage)
{
    uint32_t math_voltage = voltage*100;
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;

    frame.data[0] = 0x10;
    frame.data[1] = 0x02;
    frame.data[4] = static_cast<uint8_t>((math_voltage & 0xff000000) >> 24);
    frame.data[5] = static_cast<uint8_t>((math_voltage & 0x00ff0000) >> 16);
    frame.data[6] = static_cast<uint8_t>((math_voltage & 0x0000ff00) >> 8);
    frame.data[7] = static_cast<uint8_t>(math_voltage & 0x000000ff);
    return frame;
}

can_frame MMeetFrameGenerator::generateCurrentSet(uint8_t module_address, uint16_t current)
{
    uint32_t math_current = current*100;
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;

    frame.data[0] = 0x10;
    frame.data[1] = 0x03;
    frame.data[4] = static_cast<uint8_t>((math_current & 0xff000000) >> 24);
    frame.data[5] = static_cast<uint8_t>((math_current & 0x00ff0000) >> 16);
    frame.data[6] = static_cast<uint8_t>((math_current & 0x0000ff00) >> 8);
    frame.data[7] = static_cast<uint8_t>(math_current & 0x000000ff);
    return frame;
}

can_frame MMeetFrameGenerator::generateEnable(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x10;
    frame.data[1] = 0x04;
    frame.data[7] = ON;
    return frame;
}

can_frame MMeetFrameGenerator::generateDisable(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x10;
    frame.data[1] = 0x04;
    frame.data[7] = OFF;
    return frame;
}