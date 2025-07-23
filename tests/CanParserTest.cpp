
#include <gtest/gtest.h>
#include "../libmodul.h"

class CanParserTest : public ::testing::Test {
protected:
    CanParser parser;
    
    can_frame createUUgreenFrame(uint32_t id, uint8_t cmd, uint32_t data) {
        can_frame frame;
        frame.can_id = (id << 14) | UUGREEN_MASK;
        frame.can_dlc = CAN_INV_DLC;
        frame.data[0] = 0xAA; // preamble
        frame.data[1] = cmd;
        frame.data[2] = 0;
        frame.data[3] = 0;
        frame.data[4] = (data >> 24) & 0xFF;
        frame.data[5] = (data >> 16) & 0xFF;
        frame.data[6] = (data >> 8) & 0xFF;
        frame.data[7] = data & 0xFF;
        return frame;
    }
    
    can_frame createMMeetFrame(uint32_t id, uint16_t cmd, uint32_t data) {
        can_frame frame;
        frame.can_id = (id << 3) | MMEET_ID;
        frame.can_dlc = CAN_INV_DLC;
        frame.data[0] = 0x55; // preamble
        frame.data[1] = 0xAA; // preamble
        frame.data[2] = (cmd >> 8) & 0xFF;
        frame.data[3] = cmd & 0xFF;
        frame.data[4] = (data >> 24) & 0xFF;
        frame.data[5] = (data >> 16) & 0xFF;
        frame.data[6] = (data >> 8) & 0xFF;
        frame.data[7] = data & 0xFF;
        return frame;
    }
};

// Тесты для протокола UUgreen
TEST_F(CanParserTest, UUgreen_ValidVoltage) {
    uint32_t address = 0x12;
    uint32_t voltage = 123456; // 123.456V
    auto frame = createUUgreenFrame(address, 0x00, voltage);
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::VOLTAGE));
    EXPECT_EQ(data->address, address);
    EXPECT_FLOAT_EQ(data->voltage, 123.456f);
}

TEST_F(CanParserTest, UUgreen_ValidCurrent) {
    uint32_t address = 0x34;
    uint32_t current = 45678; // 45.678A
    auto frame = createUUgreenFrame(address, 0x01, current);
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::CURRENT));
    EXPECT_EQ(data->address, address);
    EXPECT_FLOAT_EQ(data->current, 45.678f);
}

TEST_F(CanParserTest, UUgreen_ValidTemperature) {
    uint32_t address = 0x56;
    uint32_t temp = 25000; // 25.000°C
    auto frame = createUUgreenFrame(address, 0x1E, temp);
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::TEMP));
    EXPECT_EQ(data->address, address);
    EXPECT_EQ(data->temperature, 25);
}

TEST_F(CanParserTest, UUgreen_ValidStatus) {
    uint32_t address = 0x78;
    uint32_t status = 0xABCD1234;
    auto frame = createUUgreenFrame(address, 0x08, status);
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::STATUS));
    EXPECT_EQ(data->address, address);
    EXPECT_EQ(data->status, status);
}

TEST_F(CanParserTest, UUgreen_ValidCurrentCapability) {
    uint32_t address = 126;
    uint32_t capability = 100000; // 100.000A
    auto frame = createUUgreenFrame(address, 0x68, capability);
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::CAPABILITY));
    EXPECT_EQ(data->address, address);
    EXPECT_FLOAT_EQ(data->current_capability, 100.0f);
}

TEST_F(CanParserTest, UUgreen_UnknownCommand) {
    uint32_t address = 0xBC;
    auto frame = createUUgreenFrame(address, 0x99, 0x12345678);
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::UNKNOWN_CMD);
    ASSERT_FALSE(data);
}

TEST_F(CanParserTest, UUgreen_InvalidFrame) {
    can_frame frame;
    frame.can_id = 0x123; // Invalid ID
    frame.can_dlc = 5;    // Invalid DLC
    
    auto [data, result] = parser.parse(frame, ProtocolType::UUgreen);
    
    ASSERT_EQ(result, ParseResult::INVALID_FRAME);
    ASSERT_FALSE(data);
}

// Тесты для протокола MMeet
TEST_F(CanParserTest, MMeet_ValidVoltage) {
    uint32_t address = 0x12;
    uint32_t voltage = 54321; // 54.321V
    auto frame = createMMeetFrame(address, 0x0231, voltage);
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::VOLTAGE));
    EXPECT_EQ(data->address, address);
    EXPECT_FLOAT_EQ(data->voltage, 54.321f);
}

TEST_F(CanParserTest, MMeet_ValidCurrent) {
    uint32_t address = 0x34;
    uint32_t current = 12345; // 12.345A
    auto frame = createMMeetFrame(address, 0x0232, current);
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::CURRENT));
    EXPECT_EQ(data->address, address);
    EXPECT_FLOAT_EQ(data->current, 12.345f);
}

TEST_F(CanParserTest, MMeet_ValidTemperature) {
    uint32_t address = 0x56;
    uint32_t temp = 423; // 42.3°C
    auto frame = createMMeetFrame(address, 0x020B, temp);
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::TEMP));
    EXPECT_EQ(data->address, address);
    EXPECT_EQ(data->temperature, 42);
}

TEST_F(CanParserTest, MMeet_ValidStatus) {
    uint32_t address = 0x78;
    uint32_t status = 0xDEADBEEF;
    auto frame = createMMeetFrame(address, 0x0218, status);
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::STATUS));
    EXPECT_EQ(data->address, address);
    EXPECT_EQ(data->status, status);
}

TEST_F(CanParserTest, MMeet_ValidCurrentCapability) {
    uint32_t address = 0x9A;
    uint32_t capability = 150; // 150A
    auto frame = createMMeetFrame(address, 0x0235, capability);
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::OK);
    ASSERT_TRUE(data);
    ASSERT_TRUE(data->fields.test(ParsedData::ADDR));
    ASSERT_TRUE(data->fields.test(ParsedData::CAPABILITY));
    EXPECT_EQ(data->address, address);
    EXPECT_FLOAT_EQ(data->current_capability, 150.0f);
}

TEST_F(CanParserTest, MMeet_UnknownCommand) {
    uint32_t address = 0xBC;
    auto frame = createMMeetFrame(address, 0x9999, 0x12345678);
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::UNKNOWN_CMD);
    ASSERT_FALSE(data);
}

TEST_F(CanParserTest, MMeet_InvalidFrame) {
    can_frame frame;
    frame.can_id = 0x123; // Invalid ID
    frame.can_dlc = 5;    // Invalid DLC
    
    auto [data, result] = parser.parse(frame, ProtocolType::MMeet);
    
    ASSERT_EQ(result, ParseResult::INVALID_FRAME);
    ASSERT_FALSE(data);
}

// Тест на обработку неизвестного протокола
TEST_F(CanParserTest, UnknownProtocol) {
    can_frame frame;
    frame.can_id = 0x123;
    frame.can_dlc = 8;
    
    auto [data, result] = parser.parse(frame, static_cast<ProtocolType>(99));
    
    ASSERT_EQ(result, ParseResult::INVALID_FRAME);
    ASSERT_FALSE(data);
}