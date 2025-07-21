#include "../include/libmodul.h"

#define CAN_INV_EFF_FLAG                0x80000000
 
const uint32_t LOW_MODE = 0x1111;                
const uint32_t HIGH_MODE = 0x2222; 
const uint32_t AUTO_MODE = 0x0;           
const uint32_t OFF = 0x55;                     
const uint32_t ON = 0xAA;
const uint8_t P2P_COMMUNICATION = 0x01;

can_frame MMeetFrameGenerator::init_frame(uint8_t module_address)
{
    can_frame frame{};
    frame.can_dlc = CAN_INV_DLC;
    frame.can_id = (0x60 << 20 | P2P_COMMUNICATION << 19 | module_address << 11 | 0xF0 << 3 | 0x03);           
    frame.can_id |= CAN_INV_EFF_FLAG;
    return frame;
}

can_frame MMeetFrameGenerator::generateTempRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x02;
    frame.data[3] = 0x0B;
    return frame;

    
}

can_frame MMeetFrameGenerator::generateCurrentCapabilityRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x02;
    frame.data[3] = 0x35;
    return frame;
}
    
can_frame MMeetFrameGenerator::generateFlagsRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x02;
    frame.data[3] = 0x18;
    return frame;
}

can_frame MMeetFrameGenerator::generateVoltageRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x02;
    frame.data[3] = 0x31;
    return frame;
}

can_frame MMeetFrameGenerator::generateCurrentRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x02;
    frame.data[3] = 0x32;
    return frame;
}

can_frame MMeetFrameGenerator::generateLowModeSet(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[3] = 0x5D;
    frame.data[6] = static_cast<uint8_t>((LOW_MODE & 0xFF00) >> 8);
    frame.data[7] = static_cast<uint8_t>(LOW_MODE & 0xFF);
    return frame;
}

can_frame MMeetFrameGenerator::generateHighModeSet(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[3] = 0x5D;
    frame.data[6] = static_cast<uint8_t>((HIGH_MODE & 0xFF00) >> 8);
    frame.data[7] = static_cast<uint8_t>(HIGH_MODE & 0xFF);
    return frame;
}

std::optional<can_frame> MMeetFrameGenerator::generateAutoModeSet(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[3] = 0x5D;
    frame.data[6] = static_cast<uint8_t>((AUTO_MODE & 0xFF00) >> 8);
    frame.data[7] = static_cast<uint8_t>(AUTO_MODE & 0xFF);
    return frame;
}
   
can_frame MMeetFrameGenerator::generateVoltageSet(uint8_t module_address, uint16_t voltage)
{
    uint32_t math_voltage = voltage*100;
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;

    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x2C;
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

    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[2] = 0x2D;
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
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[3] = 0x01;
    frame.data[7] = ON;
    return frame;
}

can_frame MMeetFrameGenerator::generateDisable(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x01;
    frame.data[1] = 0xF0;
    frame.data[3] = 0x01;
    frame.data[7] = OFF;
    return frame;
}