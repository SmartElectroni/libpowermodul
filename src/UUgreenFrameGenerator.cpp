#include "../include/libmodul.h"

can_frame UUgreenFrameGenerator::init_frame(uint8_t module_address)
{
    can_frame frame{};
    frame.can_dlc = CAN_INV_DLC;
    frame.can_id = (0x01 << 25 | 0x01 << 21 | (module_address & 0b01111111) << 14 | 0x00 << 9 | 0x00);       
    frame.can_id |= CAN_INV_EFF_FLAG;
}

can_frame UUgreenFrameGenerator::generateTempRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x1E;
    return frame;
}

can_frame UUgreenFrameGenerator::generateCurrentCapabilityRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x68;
    return frame;
}
    
can_frame UUgreenFrameGenerator::generateFlagsRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x08;
    return frame;
}

can_frame UUgreenFrameGenerator::generateVoltageRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x62;
    return frame;
}

can_frame UUgreenFrameGenerator::generateCurrentRequest(uint8_t module_address)
{
    can_frame frame{};
    frame.can_id = (init_frame(module_address)).can_id;
    frame.data[0] = 0x12;
    frame.data[1] = 0x30;
    return frame;
}
   
can_frame UUgreenFrameGenerator::generateVoltageSet(uint8_t module_address, uint16_t voltage)
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