/* MIT License Copyright (c) 2025 SmartElectroni*/
#include "../libmodul.h"

namespace MMeetConstants {
    constexpr uint8_t P2P_COMMUNICATION = 0x01;
    constexpr uint8_t FRAME_PREFIX = 0x01;
    constexpr uint8_t FRAME_SUFFIX = 0xF0;
    
    // cmd request
    constexpr uint16_t TEMP_CMD = 0x020B;
    constexpr uint16_t CURRENT_CAP_CMD = 0x0235;
    constexpr uint16_t FLAGS_CMD = 0x0218;
    constexpr uint16_t VOLTAGE_CMD = 0x0231;
    constexpr uint16_t CURRENT_CMD = 0x0232;
    
    // cmd control
    constexpr uint16_t MODE_SET_CMD = 0x025D;
    constexpr uint16_t VOLTAGE_SET_CMD = 0x022C;
    constexpr uint16_t CURRENT_SET_CMD = 0x022D;
    constexpr uint16_t POWER_CTRL_CMD = 0x0201;
    
    // mode and state
    constexpr uint16_t LOW_MODE = 0x1111;
    constexpr uint16_t HIGH_MODE = 0x2222;
    constexpr uint16_t AUTO_MODE = 0x0000;
    constexpr uint8_t OFF = 0x55;
    constexpr uint8_t ON = 0xAA;
    
    constexpr uint32_t MASK = 0x6000000;
    constexpr uint8_t MAX_ADDRESS = 0x7F;
}

namespace {
    constexpr uint32_t CAN_INV_EFF_FLAG = 0x80000000;
}

can_frame MMeetFrameGenerator::init_frame(uint8_t module_address) {
    can_frame frame{};
    frame.can_dlc = CAN_INV_DLC;
    frame.can_id = (MMeetConstants::MASK | MMeetConstants::P2P_COMMUNICATION << 19 | 
                   (module_address & MMeetConstants::MAX_ADDRESS) << 11 | 0xF0 << 3 | 0x03);
    frame.can_id |= CAN_INV_EFF_FLAG;
    return frame;
}

can_frame MMeetFrameGenerator::create_command_frame(uint8_t module_address, uint16_t command) {
    can_frame frame = init_frame(module_address);
    frame.data[0] = MMeetConstants::FRAME_PREFIX;
    frame.data[1] = MMeetConstants::FRAME_SUFFIX;
    frame.data[2] = static_cast<uint8_t>(command >> 8);
    frame.data[3] = static_cast<uint8_t>(command);
    return frame;
}

can_frame MMeetFrameGenerator::create_control_frame(uint8_t module_address, uint16_t command, uint16_t value) {
    can_frame frame = create_command_frame(module_address, command);
    frame.data[6] = static_cast<uint8_t>(value >> 8);
    frame.data[7] = static_cast<uint8_t>(value);
    return frame;
}

void MMeetFrameGenerator::pack_uint32(uint8_t* dest, uint32_t value) {
    dest[0] = static_cast<uint8_t>(value >> 24);
    dest[1] = static_cast<uint8_t>(value >> 16);
    dest[2] = static_cast<uint8_t>(value >> 8);
    dest[3] = static_cast<uint8_t>(value);
}

can_frame MMeetFrameGenerator::generateTempRequest(uint8_t module_address) {
    return create_command_frame(module_address, MMeetConstants::TEMP_CMD);
}

can_frame MMeetFrameGenerator::generateCurrentCapabilityRequest(uint8_t module_address) {
    return create_command_frame(module_address, MMeetConstants::CURRENT_CAP_CMD);
}

can_frame MMeetFrameGenerator::generateFlagsRequest(uint8_t module_address) {
    return create_command_frame(module_address, MMeetConstants::FLAGS_CMD);
}

can_frame MMeetFrameGenerator::generateVoltageRequest(uint8_t module_address) {
    return create_command_frame(module_address, MMeetConstants::VOLTAGE_CMD);
}

can_frame MMeetFrameGenerator::generateCurrentRequest(uint8_t module_address) {
    return create_command_frame(module_address, MMeetConstants::CURRENT_CMD);
}

can_frame MMeetFrameGenerator::generateLowModeSet(uint8_t module_address) {
    return create_control_frame(module_address, MMeetConstants::MODE_SET_CMD, MMeetConstants::LOW_MODE);
}

can_frame MMeetFrameGenerator::generateHighModeSet(uint8_t module_address) {
    return create_control_frame(module_address, MMeetConstants::MODE_SET_CMD, MMeetConstants::HIGH_MODE);
}

std::optional<can_frame> MMeetFrameGenerator::generateAutoModeSet(uint8_t module_address) {
    return create_control_frame(module_address, MMeetConstants::MODE_SET_CMD, MMeetConstants::AUTO_MODE);
}

can_frame MMeetFrameGenerator::generateVoltageSet(uint8_t module_address, float voltage) {
    uint32_t math_voltage = static_cast<uint32_t>(voltage * 100);
    can_frame frame = create_command_frame(module_address, MMeetConstants::VOLTAGE_SET_CMD);
    pack_uint32(frame.data + 4, math_voltage);
    return frame;
}

can_frame MMeetFrameGenerator::generateCurrentSet(uint8_t module_address, float current) {
    uint32_t math_current = static_cast<uint32_t>(current * 1000);
    can_frame frame = create_command_frame(module_address, MMeetConstants::CURRENT_SET_CMD);
    pack_uint32(frame.data + 4, math_current);
    return frame;
}

can_frame MMeetFrameGenerator::generateEnable(uint8_t module_address) {
    can_frame frame = create_command_frame(module_address, MMeetConstants::POWER_CTRL_CMD);
    frame.data[7] = MMeetConstants::ON;
    return frame;
}

can_frame MMeetFrameGenerator::generateDisable(uint8_t module_address) {
    can_frame frame = create_command_frame(module_address, MMeetConstants::POWER_CTRL_CMD);
    frame.data[7] = MMeetConstants::OFF;
    return frame;
}
