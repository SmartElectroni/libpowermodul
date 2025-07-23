#pragma once
#include <cstdint>
#include <memory>
#include <array>
#include <optional>
#include <bitset>

namespace {
    constexpr uint8_t CAN_INV_DLC = 8;
    constexpr uint32_t UUGREEN_MASK = 0x2000000;
    constexpr uint32_t MMEET_MASK = 0xFFFF0000;
    constexpr uint32_t MMEET_ID = 0x060F0000;
}


#pragma pack(push, 1)
/**
 * @brief CAN frame structure
 */
struct can_frame {
    uint32_t can_id;  
    uint8_t can_dlc;
    uint8_t data[8];
};
#pragma pack(pop)

/**
 * @brief Protocol types enumeration
 */
enum class ProtocolType {
    UUgreen,
    MMeet
    // Add other protocols as needed
};

/**
 * @brief Abstract strategy for CAN frame generation
 */
class ICanFrameGenerator {
public:
    virtual ~ICanFrameGenerator() = default;
    
    /**
     * @brief Generate CAN frame for temperature reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateTempRequest(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for Current capability reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateCurrentCapabilityRequest(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for Flags reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateFlagsRequest(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for voltage reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateVoltageRequest(uint8_t module_address) = 0;
    
    /**
     * @brief Generate CAN frame for current reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateCurrentRequest(uint8_t module_address) = 0;
    
    /**
     * @brief Generate CAN frame for set low mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateLowModeSet(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for set high mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateHighModeSet(uint8_t module_address) = 0; 

    /**
     * @brief Generate CAN frame for set auto mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual std::optional<can_frame> generateAutoModeSet(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in V units)
     * @return Generated CAN frame
     */
    virtual can_frame generateVoltageSet(uint8_t module_address, float voltage) = 0;
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in A units)
     * @return Generated CAN frame
     */
    virtual can_frame generateCurrentSet(uint8_t module_address, float current) = 0;

    /**
     * @brief Generate CAN frame for power ON
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateEnable(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for power OFF
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame generateDisable(uint8_t module_address) = 0;
};



/**
 * @brief CAN Frame Generator for UUgreen protocol
 */
class UUgreenFrameGenerator : public ICanFrameGenerator {
public:

    /**
     * @brief Generate CAN frame for temperature reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateTempRequest(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for Current capability reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateCurrentCapabilityRequest(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for Flags reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateFlagsRequest(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for voltage reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateVoltageRequest(uint8_t module_address) override;
    
    /**
     * @brief Generate CAN frame for current reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateCurrentRequest(uint8_t module_address) override;
    
    /**
     * @brief Generate CAN frame for set low mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateLowModeSet(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for set high mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateHighModeSet(uint8_t module_address) override; 

    /**
     * @brief Not supported
     * @param module_address Device address
     * @return std::nullopt
     */
    std::optional<can_frame> generateAutoModeSet(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in V units)
     * @return Generated CAN frame
     */
    can_frame generateVoltageSet(uint8_t module_address, float voltage) override;
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in A units)
     * @return Generated CAN frame
     */
    can_frame generateCurrentSet(uint8_t module_address, float current) override;

    /**
     * @brief Generate CAN frame for power ON
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateEnable(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for power OFF
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateDisable(uint8_t module_address) override;

private:
    /**
     * @brief Init CAN frame for create request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame init_frame (uint8_t module_address); 
    
    can_frame create_command_frame(uint8_t module_address, uint8_t prefix, uint8_t command);
    can_frame create_control_frame(uint8_t module_address, uint8_t command, uint8_t value = 0);
    void pack_uint32(uint8_t* dest, uint32_t value);
    
};

/**
 * @brief CAN Frame Generator for MMeet protocol
 */
class MMeetFrameGenerator : public ICanFrameGenerator {
public:

    /**
     * @brief Generate CAN frame for temperature reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateTempRequest(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for Current capability reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateCurrentCapabilityRequest(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for Flags reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateFlagsRequest(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for voltage reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateVoltageRequest(uint8_t module_address) override;
    
    /**
     * @brief Generate CAN frame for current reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateCurrentRequest(uint8_t module_address) override;
    
    /**
     * @brief Generate CAN frame for set low mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateLowModeSet(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for set high mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateHighModeSet(uint8_t module_address) override; 

    /**
     * @brief Generate CAN frame for set auto mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    std::optional<can_frame> generateAutoModeSet(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in V units)
     * @return Generated CAN frame
     */
    can_frame generateVoltageSet(uint8_t module_address, float voltage) override;
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in A units)
     * @return Generated CAN frame
     */
    can_frame generateCurrentSet(uint8_t module_address, float current) override;

    /**
     * @brief Generate CAN frame for power ON
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateEnable(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for power OFF
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateDisable(uint8_t module_address) override;

protected:
    /**
     * @brief Init CAN frame for create request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame init_frame (uint8_t module_address);    
};

/**
 * @brief CAN Protocol Manager (Context)
 */
class CanProtocolManager {
public:
    /**
     * @brief Constructor with protocol selection
     * @param protocol Type of protocol to use
     */
    explicit CanProtocolManager(ProtocolType protocol) {
        switch(protocol) {
            case ProtocolType::UUgreen:
                _generator = std::make_unique<UUgreenFrameGenerator>();
                break;
            case ProtocolType::MMeet:
                _generator = std::make_unique<MMeetFrameGenerator>();
                break;
            // Add other protocols...
        }
    }
    
    /**
     * @brief Set protocol at runtime
     * @param protocol New protocol type
     */
    void setProtocol(ProtocolType protocol) {
        switch(protocol) {
            case ProtocolType::UUgreen:
                _generator = std::make_unique<UUgreenFrameGenerator>();
                break;
            case ProtocolType::MMeet:
                _generator = std::make_unique<MMeetFrameGenerator>();
                break;
            // Add other protocols...
        }
    }

    /**
     * @brief Generate CAN frame for temperature reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateTempRequest(uint8_t module_address) {
        return _generator->generateTempRequest(module_address);
    }

    /**
     * @brief Generate CAN frame for Current capability reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateCurrentCapabilityRequest(uint8_t module_address) {
        return _generator->generateCurrentCapabilityRequest(module_address);
    }

    /**
     * @brief Generate CAN frame for Flags reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateFlagsRequest(uint8_t module_address) {
        return _generator->generateFlagsRequest(module_address);
    }

    /**
     * @brief Generate CAN frame for voltage reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateVoltageRequest(uint8_t module_address) {
        return _generator->generateVoltageRequest(module_address);
    } 
    
    /**
     * @brief Generate CAN frame for current reading request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateCurrentRequest(uint8_t module_address) {
        return _generator->generateCurrentRequest(module_address);
    } 
    
    /**
     * @brief Generate CAN frame for set low mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateLowModeSet(uint8_t module_address) {
        return _generator->generateLowModeSet(module_address);
    } 

    /**
     * @brief Generate CAN frame for set high mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateHighModeSet(uint8_t module_address) {
        return _generator->generateHighModeSet(module_address);
    } 

    /**
     * @brief Generate CAN frame for set auto mode request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    std::optional<can_frame> generateAutoModeSet(uint8_t module_address) {
        return _generator->generateAutoModeSet(module_address);
    }

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in V units)
     * @return Generated CAN frame
     */
    can_frame generateVoltageSet(uint8_t module_address, float voltage) {
        return _generator->generateVoltageSet(module_address , voltage);
    }
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in A units)
     * @return Generated CAN frame
     */
    can_frame generateCurrentSet(uint8_t module_address, float current) {
        return _generator->generateCurrentSet(module_address , current);
    }

    /**
     * @brief Generate CAN frame for power ON
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateEnable(uint8_t module_address) {
        return _generator->generateEnable(module_address);
    }

    /**
     * @brief Generate CAN frame for power OFF
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame generateDisable(uint8_t module_address) {
        return _generator->generateDisable(module_address);
    }
    
private:
    std::unique_ptr<ICanFrameGenerator> _generator;
};

#pragma pack(push,1)
struct ParsedData {
    uint8_t address;
    float voltage = 0.0f;
    float current = 0.0f;
    int16_t temperature = 0;
    uint32_t status = 0;
    float current_capability = 0.0f;

    enum Field { ADDR, VOLTAGE, CURRENT, TEMP, STATUS, CAPABILITY, COUNT };
    std::bitset<COUNT> fields;
    
    explicit operator bool() const { return fields.any(); }
};
#pragma pack(pop)

enum class ParseResult { OK, UNKNOWN_CMD, INVALID_FRAME };

class CanParser {
public:
    /**
     * @brief Parsing CAN frame
     * @param frame CAN frame for parsing
     * @param protocol Type protocol for interpretation
     * @return  std::pair<std::optional<ParsedData>, ParseResult> data
     */
    std::pair<std::optional<ParsedData>, ParseResult> parse(can_frame frame, ProtocolType protocol);
    

private:

    /**
     * @brief Get 32-bit data for choose protocol
     * @param frame Link to CAN-frame
     * @param start_byte start position
     * @return uint32_t Got value 
     */
    uint32_t extractData(const can_frame& frame, uint8_t start_byte = 4) const;

    /**
     * @brief Check valid frame for choose protocol
     * @param frame Link to CAN-frame
     * @param mask mask for choose protocol
     * @param expected expected for choose protocol
     * @return bool isValid 
     */
    bool validateFrame(const can_frame& frame, uint32_t mask, uint32_t expected) const;
    
     /**
     * @brief Parsing frame for protocol UUgreen
     * @param frame CAN-frame (send to link)
     * @return std::pair<std::optional<ParsedData>, ParseResult> Parsed data or std::nullopt for error
     */
    std::pair<std::optional<ParsedData>, ParseResult> parseUUgreen(can_frame frame);

    /**
     * @brief Parsing frame for protocol MMeet
     * @param frame CAN-frame (send to link)
     * @return std::pair<std::optional<ParsedData>, ParseResult> Parsed data or std::nullopt for error
     */
    std::pair<std::optional<ParsedData>, ParseResult> parseMMeet(can_frame frame);

    // Add other protocol ...
};

