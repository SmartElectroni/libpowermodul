#include "../include/libmodul.h"

uint32_t CanParser::get_dataUUgreen(const struct can_frame& frame){
    return (uint32_t)((frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | (frame.data[7]));    
}
std::optional<ParsedData> CanParser::parseUUgreen(can_frame& frame) {
    ParsedData result;
    if (((frame.can_id & 0x2000000) == 0x2000000) && (frame.can_dlc == CAN_INV_DLC))
    {
        frame.data[2] = 0;
        frame.data[3] = 0;

        result.address = (frame.can_id & 0x1FC000) >> 14; 

        switch(frame.data[1]) 
        {
        case 0x00: case 0x62:
            result.voltage = (float)(get_dataUUgreen(frame) / 1000.0f);
            break;
        case 0x01: case 0x30:
            result.current = (float)(get_dataUUgreen(frame) / 1000.0f);
            break;
        case 0x08:
            result.status = get_dataUUgreen(frame);
            break;
        case 0x1E:
            result.temperature = (int16_t)( ( (int32_t)get_dataUUgreen(frame) )/1000);
            break;
        case 0x68:
            result.current_capability = (float)(get_dataUUgreen(frame) / 1000.0f);
            break;
        default:
            break;
        }
        return result;
    }
    return std::nullopt;
}

uint32_t CanParser::get_dataMMeet(const struct can_frame& frame){
    return (uint32_t)((frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | (frame.data[7]));    
}

std::optional<ParsedData> CanParser::parseMMeet(const can_frame& frame) {
    ParsedData result;
    if (((frame.can_id & 0xFFFF0000) == 0x060F0000) && (frame.can_dlc == CAN_INV_DLC))
    {
        result.address = (frame.can_id & 0x7F8) >> 3;

        switch((uint16_t)((frame.data[2] << 8) | (frame.data[3])))
        {
        case 0x0231:
            result.voltage = (float)(get_dataMMeet(frame) / 1000.0f);
            break;
        case 0x0232:
            result.current = (float)(get_dataMMeet(frame) / 1000.0f);
            break;
        case 0x0218:
            result.status = get_dataMMeet(frame);
            break;
        case 0x020B:
            result.temperature = (int16_t)(((int32_t)get_dataMMeet(frame)) / 10);
            break;
        case 0x0235:
            result.current_capability = (uint16_t)(get_dataMMeet(frame));
            break;
        default:
            break;
        }
        return result;
    }
    return std::nullopt;
}
