/* MIT License Copyright (c) 2025 SmartElectroni*/
#include "../libmodul.h"

namespace UUgreenConstants {
    constexpr uint8_t PREAMBLE = 0x12;
    constexpr uint8_t CONTROL_PREFIX = 0x10;
    
    // cmd request
    constexpr uint8_t TEMP_CMD = 0x1E;
    constexpr uint8_t CURRENT_CAP_CMD = 0x68;
    constexpr uint8_t FLAGS_CMD = 0x08;
    constexpr uint8_t VOLTAGE_CMD = 0x62;
    constexpr uint8_t CURRENT_CMD = 0x30;
    
    // cmd control
    constexpr uint8_t MODE_SET_CMD = 0x5F;
    constexpr uint8_t VOLTAGE_SET_CMD = 0x02;
    constexpr uint8_t CURRENT_SET_CMD = 0x03;
    constexpr uint8_t POWER_CTRL_CMD = 0x04;
    
    // mode and state
    constexpr uint8_t LOW_MODE = 0x02;
    constexpr uint8_t HIGH_MODE = 0x01;
    constexpr uint8_t OFF = 0x01;
    constexpr uint8_t ON = 0x00;
    
    constexpr uint32_t MASK = 0x02200000;
    constexpr uint8_t MAX_ADDRESS = 0x7F;
}

namespace {
    constexpr uint32_t CAN_INV_EFF_FLAG = 0x80000000;
}

can_frame UUgreenFrameGenerator::init_frame(uint8_t module_address) {
    can_frame frame{};
    frame.can_dlc = CAN_INV_DLC;
    frame.can_id = UUgreenConstants::MASK | ((module_address & UUgreenConstants::MAX_ADDRESS) << 14);
    frame.can_id |= CAN_INV_EFF_FLAG;
    return frame;
}

can_frame UUgreenFrameGenerator::create_command_frame(uint8_t module_address, uint8_t prefix, uint8_t command) {
    can_frame frame = init_frame(module_address);
    frame.data[0] = prefix;
    frame.data[1] = command;
    return frame;
}

can_frame UUgreenFrameGenerator::create_control_frame(uint8_t module_address, uint8_t command, uint8_t value) {
    can_frame frame = create_command_frame(module_address, UUgreenConstants::CONTROL_PREFIX, command);
    frame.data[7] = value;
    return frame;
}

void UUgreenFrameGenerator::pack_uint32(uint8_t* dest, uint32_t value) {
    dest[0] = static_cast<uint8_t>(value >> 24);
    dest[1] = static_cast<uint8_t>(value >> 16);
    dest[2] = static_cast<uint8_t>(value >> 8);
    dest[3] = static_cast<uint8_t>(value);
}

can_frame UUgreenFrameGenerator::generateTempRequest(uint8_t module_address) {
    return create_command_frame(module_address, UUgreenConstants::PREAMBLE, UUgreenConstants::TEMP_CMD);
}

can_frame UUgreenFrameGenerator::generateCurrentCapabilityRequest(uint8_t module_address) {
    return create_command_frame(module_address, UUgreenConstants::PREAMBLE, UUgreenConstants::CURRENT_CAP_CMD);
}

can_frame UUgreenFrameGenerator::generateFlagsRequest(uint8_t module_address) {
    return create_command_frame(module_address, UUgreenConstants::PREAMBLE, UUgreenConstants::FLAGS_CMD);
}

can_frame UUgreenFrameGenerator::generateVoltageRequest(uint8_t module_address) {
    return create_command_frame(module_address, UUgreenConstants::PREAMBLE, UUgreenConstants::VOLTAGE_CMD);
}

can_frame UUgreenFrameGenerator::generateCurrentRequest(uint8_t module_address) {
    return create_command_frame(module_address, UUgreenConstants::PREAMBLE, UUgreenConstants::CURRENT_CMD);
}

can_frame UUgreenFrameGenerator::generateLowModeSet(uint8_t module_address) {
    return create_control_frame(module_address, UUgreenConstants::MODE_SET_CMD, UUgreenConstants::LOW_MODE);
}

can_frame UUgreenFrameGenerator::generateHighModeSet(uint8_t module_address) {
    return create_control_frame(module_address, UUgreenConstants::MODE_SET_CMD, UUgreenConstants::HIGH_MODE);
}

std::optional<can_frame> UUgreenFrameGenerator::generateAutoModeSet(uint8_t module_address) {
    (void)module_address;
    return std::nullopt;
}

can_frame UUgreenFrameGenerator::generateVoltageSet(uint8_t module_address, float voltage) {
    uint32_t math_voltage = static_cast<uint32_t>(voltage*1000);
    auto frame = create_command_frame(module_address, UUgreenConstants::CONTROL_PREFIX, UUgreenConstants::VOLTAGE_SET_CMD);
    pack_uint32(frame.data + 4, math_voltage);
    return frame;
}

can_frame UUgreenFrameGenerator::generateCurrentSet(uint8_t module_address, float current) {
    uint32_t math_current = static_cast<uint32_t>(current*1000);
    auto frame = create_command_frame(module_address, UUgreenConstants::CONTROL_PREFIX, UUgreenConstants::CURRENT_SET_CMD);
    pack_uint32(frame.data + 4, math_current);
    return frame;
}

can_frame UUgreenFrameGenerator::generateEnable(uint8_t module_address) {
    return create_control_frame(module_address, UUgreenConstants::POWER_CTRL_CMD, UUgreenConstants::ON);
}

can_frame UUgreenFrameGenerator::generateDisable(uint8_t module_address) {
    return create_control_frame(module_address, UUgreenConstants::POWER_CTRL_CMD, UUgreenConstants::OFF);
}
