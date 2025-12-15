#include "../libmodul.h"

uint32_t CanParser::extractData(const can_frame_lib& frame, uint8_t start_byte) const {
    return (frame.can_dlc >= start_byte + 4)
        ? (frame.data[start_byte] << 24) | (frame.data[start_byte+1] << 16) 
          | (frame.data[start_byte+2] << 8) | frame.data[start_byte+3]
        : 0;
}

bool CanParser::validateFrame(const can_frame_lib& frame, uint32_t mask, uint32_t expected) const {
    return (frame.can_id & mask) == expected && frame.can_dlc == CAN_INV_DLC;
}

std::pair<std::optional<ParsedData>, ParseResult> CanParser::parseUUgreen(can_frame_lib frame) {
    if (!validateFrame(frame, UUGREEN_MASK, UUGREEN_MASK)) 
        return {std::nullopt, ParseResult::INVALID_FRAME};

    ParsedData result;

    frame.data[2] = 0;
    frame.data[3] = 0;

    result.address = (frame.can_id & 0x1FC000) >> 14;
    result.fields.set(ParsedData::ADDR);

    const uint32_t data = extractData(frame);

    switch(frame.data[1]) {
        case 0x00: case 0x62: 
            result.voltage = data * 0.001f;
            result.fields.set(ParsedData::VOLTAGE);
            break;
        case 0x01: case 0x30:
            result.current = data * 0.001f;
            result.fields.set(ParsedData::CURRENT);
            break;
        case 0x08:
            result.status = data;
            result.fields.set(ParsedData::STATUS);
            break;
        case 0x1E:
            result.temperature = static_cast<int16_t>(data * 0.001f);
            result.fields.set(ParsedData::TEMP);
            break;
        case 0x68:
            result.current_capability = data * 0.001f;
            result.fields.set(ParsedData::CAPABILITY);
            break;
        default:
            return {std::nullopt, ParseResult::UNKNOWN_CMD};
    }
    return {result, ParseResult::OK};
}


std::pair<std::optional<ParsedData>, ParseResult> CanParser::parseMMeet(can_frame_lib frame){
    if (!validateFrame(frame, MMEET_MASK, MMEET_ID)) 
        return {std::nullopt, ParseResult::INVALID_FRAME};
    
    ParsedData result;
    result.address = (frame.can_id & 0x7F8) >> 3;
    result.fields.set(ParsedData::ADDR);

    const uint32_t data = extractData(frame);
    const uint16_t command = (frame.data[2] << 8) | frame.data[3];

    switch(command) {
        case 0x0231:
            result.voltage = data * 0.001f;
            result.fields.set(ParsedData::VOLTAGE);
            break;
        case 0x0232:
            result.current = data * 0.001f;
            result.fields.set(ParsedData::CURRENT);
            break;
        case 0x0218:
            result.status = data;
            result.fields.set(ParsedData::STATUS);
            break;
        case 0x020B:
            result.temperature = static_cast<int16_t>(data * 0.1f);
            result.fields.set(ParsedData::TEMP);
            break;
        case 0x0235:
            result.current_capability = static_cast<float>(data * 0.1f);
            result.fields.set(ParsedData::CAPABILITY);
            break;
        default:
            return {std::nullopt, ParseResult::UNKNOWN_CMD};
    }
    return {result, ParseResult::OK};
}

std::pair<std::optional<ParsedData>, ParseResult> CanParser::parse(can_frame_lib frame, ProtocolType protocol) {
    switch(protocol) {
        case ProtocolType::UUgreen: return parseUUgreen(std::move(frame));
        case ProtocolType::MMeet: return parseMMeet(std::move(frame));
        // Add other protocols...
        default: return {std::nullopt, ParseResult::INVALID_FRAME};
    }
}