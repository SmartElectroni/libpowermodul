#include <gtest/gtest.h>
#include "../libmodul.h"

class UUgreenFrameGeneratorTest : public ::testing::Test {
protected:
    UUgreenFrameGenerator generator;
    const uint8_t testAddress = 0x1A;
};

class MMeetFrameGeneratorTest : public ::testing::Test {
protected:
    MMeetFrameGenerator generator;
    const uint8_t testAddress = 0x2B;
};

class CanParserTest : public ::testing::Test {
protected:
    CanParser parser;
};

// UUgreen Frame Generator Tests
TEST_F(UUgreenFrameGeneratorTest, GenerateTempRequest) {
    auto frame = generator.generateTempRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x12); // PREAMBLE
    EXPECT_EQ(frame.data[1], 0x1E); // TEMP_CMD
}

TEST_F(UUgreenFrameGeneratorTest, GenerateCurrentCapabilityRequest) {
    auto frame = generator.generateCurrentCapabilityRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x12); // PREAMBLE
    EXPECT_EQ(frame.data[1], 0x68); // CURRENT_CAP_CMD
}

TEST_F(UUgreenFrameGeneratorTest, GenerateFlagsRequest) {
    auto frame = generator.generateFlagsRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x12); // PREAMBLE
    EXPECT_EQ(frame.data[1], 0x08); // FLAGS_CMD
}

TEST_F(UUgreenFrameGeneratorTest, GenerateVoltageRequest) {
    auto frame = generator.generateVoltageRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x12); // PREAMBLE
    EXPECT_EQ(frame.data[1], 0x62); // VOLTAGE_CMD
}

TEST_F(UUgreenFrameGeneratorTest, GenerateCurrentRequest) {
    auto frame = generator.generateCurrentRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x12); // PREAMBLE
    EXPECT_EQ(frame.data[1], 0x30); // CURRENT_CMD
}

TEST_F(UUgreenFrameGeneratorTest, GenerateLowModeSet) {
    auto frame = generator.generateLowModeSet(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x10); // CONTROL_PREFIX
    EXPECT_EQ(frame.data[1], 0x5F); // MODE_SET_CMD
    EXPECT_EQ(frame.data[7], 0x00); // LOW_MODE
}

TEST_F(UUgreenFrameGeneratorTest, GenerateHighModeSet) {
    auto frame = generator.generateHighModeSet(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x10); // CONTROL_PREFIX
    EXPECT_EQ(frame.data[1], 0x5F); // MODE_SET_CMD
    EXPECT_EQ(frame.data[7], 0x01); // HIGH_MODE
}

TEST_F(UUgreenFrameGeneratorTest, GenerateAutoModeSet) {
    auto result = generator.generateAutoModeSet(testAddress);
    EXPECT_FALSE(result.has_value());
}

TEST_F(UUgreenFrameGeneratorTest, GenerateVoltageSet) {
    float voltage = 12.34f;
    auto frame = generator.generateVoltageSet(testAddress, voltage);
    
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x10); // CONTROL_PREFIX
    EXPECT_EQ(frame.data[1], 0x02); // VOLTAGE_SET_CMD
    
    uint32_t expected = static_cast<uint32_t>(voltage * 1000);
    uint32_t actual = (frame.data[4] << 24) | (frame.data[5] << 16) | 
                     (frame.data[6] << 8) | frame.data[7];
    EXPECT_EQ(actual, expected);
}

TEST_F(UUgreenFrameGeneratorTest, GenerateCurrentSet) {
    float current = 1.23f;
    auto frame = generator.generateCurrentSet(testAddress, current);
    
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x10); // CONTROL_PREFIX
    EXPECT_EQ(frame.data[1], 0x03); // CURRENT_SET_CMD
    
    uint32_t expected = static_cast<uint32_t>(current * 1000);
    uint32_t actual = (frame.data[4] << 24) | (frame.data[5] << 16) | 
                     (frame.data[6] << 8) | frame.data[7];
    EXPECT_EQ(actual, expected);
}

TEST_F(UUgreenFrameGeneratorTest, GenerateEnable) {
    auto frame = generator.generateEnable(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x10); // CONTROL_PREFIX
    EXPECT_EQ(frame.data[1], 0x04); // POWER_CTRL_CMD
    EXPECT_EQ(frame.data[7], 0x00); // ON
}

TEST_F(UUgreenFrameGeneratorTest, GenerateDisable) {
    auto frame = generator.generateDisable(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x10); // CONTROL_PREFIX
    EXPECT_EQ(frame.data[1], 0x04); // POWER_CTRL_CMD
    EXPECT_EQ(frame.data[7], 0x01); // OFF
}

// MMeet Frame Generator Tests
TEST_F(MMeetFrameGeneratorTest, GenerateTempRequest) {
    auto frame = generator.generateTempRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // TEMP_CMD high
    EXPECT_EQ(frame.data[3], 0x0B); // TEMP_CMD low
}

TEST_F(MMeetFrameGeneratorTest, GenerateCurrentCapabilityRequest) {
    auto frame = generator.generateCurrentCapabilityRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // CURRENT_CAP_CMD high
    EXPECT_EQ(frame.data[3], 0x35); // CURRENT_CAP_CMD low
}

TEST_F(MMeetFrameGeneratorTest, GenerateFlagsRequest) {
    auto frame = generator.generateFlagsRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // FLAGS_CMD high
    EXPECT_EQ(frame.data[3], 0x18); // FLAGS_CMD low
}

TEST_F(MMeetFrameGeneratorTest, GenerateVoltageRequest) {
    auto frame = generator.generateVoltageRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // VOLTAGE_CMD high
    EXPECT_EQ(frame.data[3], 0x31); // VOLTAGE_CMD low
}

TEST_F(MMeetFrameGeneratorTest, GenerateCurrentRequest) {
    auto frame = generator.generateCurrentRequest(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // CURRENT_CMD high
    EXPECT_EQ(frame.data[3], 0x32); // CURRENT_CMD low
}

TEST_F(MMeetFrameGeneratorTest, GenerateLowModeSet) {
    auto frame = generator.generateLowModeSet(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // MODE_SET_CMD high
    EXPECT_EQ(frame.data[3], 0x5D); // MODE_SET_CMD low
    EXPECT_EQ(frame.data[6], 0x11); // LOW_MODE high
    EXPECT_EQ(frame.data[7], 0x11); // LOW_MODE low
}

TEST_F(MMeetFrameGeneratorTest, GenerateHighModeSet) {
    auto frame = generator.generateHighModeSet(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // MODE_SET_CMD high
    EXPECT_EQ(frame.data[3], 0x5D); // MODE_SET_CMD low
    EXPECT_EQ(frame.data[6], 0x22); // HIGH_MODE high
    EXPECT_EQ(frame.data[7], 0x22); // HIGH_MODE low
}

TEST_F(MMeetFrameGeneratorTest, GenerateAutoModeSet) {
    auto result = generator.generateAutoModeSet(testAddress);
    ASSERT_TRUE(result.has_value());
    auto frame = result.value();
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[6], 0x00); // AUTO_MODE high
    EXPECT_EQ(frame.data[7], 0x00); // AUTO_MODE low
}

TEST_F(MMeetFrameGeneratorTest, GenerateVoltageSet) {
    float voltage = 56.78f;
    auto frame = generator.generateVoltageSet(testAddress, voltage);
    
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // VOLTAGE_SET_CMD high
    EXPECT_EQ(frame.data[3], 0x2C); // VOLTAGE_SET_CMD low
    
    uint32_t expected = static_cast<uint32_t>(voltage * 100);
    uint32_t actual = (frame.data[4] << 24) | (frame.data[5] << 16) | 
                     (frame.data[6] << 8) | frame.data[7];
    EXPECT_EQ(actual, expected);
}

TEST_F(MMeetFrameGeneratorTest, GenerateCurrentSet) {
    float current = 5.678f;
    auto frame = generator.generateCurrentSet(testAddress, current);
    
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // CURRENT_SET_CMD high
    EXPECT_EQ(frame.data[3], 0x2D); // CURRENT_SET_CMD low
    
    uint32_t expected = static_cast<uint32_t>(current * 1000);
    uint32_t actual = (frame.data[4] << 24) | (frame.data[5] << 16) | 
                     (frame.data[6] << 8) | frame.data[7];
    EXPECT_EQ(actual, expected);
}

TEST_F(MMeetFrameGeneratorTest, GenerateEnable) {
    auto frame = generator.generateEnable(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // POWER_CTRL_CMD high
    EXPECT_EQ(frame.data[3], 0x01); // POWER_CTRL_CMD low
    EXPECT_EQ(frame.data[7], 0xAA); // ON
}

TEST_F(MMeetFrameGeneratorTest, GenerateDisable) {
    auto frame = generator.generateDisable(testAddress);
    EXPECT_EQ(frame.can_dlc, CAN_INV_DLC);
    EXPECT_NE(frame.can_id, 0u);
    EXPECT_EQ(frame.data[0], 0x01); // FRAME_PREFIX
    EXPECT_EQ(frame.data[1], 0xF0); // FRAME_SUFFIX
    EXPECT_EQ(frame.data[2], 0x02); // POWER_CTRL_CMD high
    EXPECT_EQ(frame.data[3], 0x01); // POWER_CTRL_CMD low
    EXPECT_EQ(frame.data[7], 0x55); // OFF
}

// CanParser Tests
TEST_F(CanParserTest, ParseUUgreenVoltage) {
    can_frame_lib frame;
    frame.can_id = 0x02200000 | (0x10 << 14) | 0x80000000;
    frame.can_dlc = CAN_INV_DLC;
    frame.data[0] = 0x12; // PREAMBLE
    frame.data[1] = 0x62; // VOLTAGE_CMD
    frame.data[4] = 0x00; // 5.000V (5000)
    frame.data[5] = 0x00;
    frame.data[6] = 0x13;
    frame.data[7] = 0x88;
    
    auto result = parser.parse(frame, ProtocolType::UUgreen);
    EXPECT_EQ(result.second, ParseResult::OK);
    ASSERT_TRUE(result.first.has_value());
    EXPECT_EQ(result.first->address, 0x10);
    EXPECT_FLOAT_EQ(result.first->voltage, 5.0f);
}

TEST_F(CanParserTest, ParseMMeetCurrent) {
    can_frame_lib frame;
    frame.can_id = (0x10 << 3) | MMEET_ID;
    frame.can_dlc = CAN_INV_DLC;
    frame.data[0] = 0x01; // FRAME_PREFIX
    frame.data[1] = 0xF0; // FRAME_SUFFIX
    frame.data[2] = 0x02; // CURRENT_CMD high
    frame.data[3] = 0x32; // CURRENT_CMD low
    frame.data[4] = 0x00; // 1.500A (1500)
    frame.data[5] = 0x00;
    frame.data[6] = 0x05;
    frame.data[7] = 0xDC;
    
    auto result = parser.parse(frame, ProtocolType::MMeet);
    EXPECT_EQ(result.second, ParseResult::OK);
    ASSERT_TRUE(result.first.has_value());
    EXPECT_EQ(result.first->address, 0x10);
    EXPECT_FLOAT_EQ(result.first->current, 1.5f);
}

TEST_F(CanParserTest, ParseUnknownProtocol) {
    can_frame_lib frame;
    auto result = parser.parse(frame, static_cast<ProtocolType>(99));
    EXPECT_EQ(result.second, ParseResult::INVALID_FRAME);
}
