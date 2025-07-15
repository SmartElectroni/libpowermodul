#pragma once
#include <cstdint>
#include <memory>
#include <array>

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
     * @brief Init CAN frame for create request
     * @param module_address Device address
     * @return Generated CAN frame
     */
    virtual can_frame init_frame (uint8_t module_address) = 0;

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
    virtual can_frame generateAutoModeSet(uint8_t module_address) = 0;

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in 0.1V units)
     * @return Generated CAN frame
     */
    virtual can_frame generateVoltageSet(uint8_t module_address, uint16_t voltage) = 0;
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in 0.1A units)
     * @return Generated CAN frame
     */
    virtual can_frame generateCurrentSet(uint8_t module_address, uint16_t current) = 0;

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
    can_frame generateVoltageRequest(uint8_t module_address) override {
        can_frame frame{};
        frame.can_id = module_address;
        frame.can_dlc = 2;
        frame.data[0] = 0x12;
        frame.data[1] = 0x62;
        return frame;
    }
    
    can_frame generateCurrentRequest(uint8_t module_address) override {
        can_frame frame{};
        frame.can_id = module_address;
        frame.can_dlc = 2;
        frame.data[0] = 0x13;
        frame.data[1] = 0x63;
        return frame;
    }
    
    can_frame generateVoltageSet(uint8_t module_address, uint16_t voltage) override {
        can_frame frame{};
        frame.can_id = module_address;
        frame.can_dlc = 3;
        frame.data[0] = 0x22;
        frame.data[1] = static_cast<uint8_t>(voltage >> 8);
        frame.data[2] = static_cast<uint8_t>(voltage & 0xFF);
        return frame;
    }
};

/**
 * @brief CAN Frame Generator for MMeet protocol
 */
class MMeetFrameGenerator : public ICanFrameGenerator {
public:
    can_frame generateVoltageRequest(uint8_t module_address) override {
        can_frame frame{};
        frame.can_id = module_address;
        frame.can_dlc = 5;
        frame.data[0] = 0x01;
        frame.data[1] = 0xF0;
        frame.data[2] = 0x00;
        frame.data[3] = 0x02;
        frame.data[4] = 0x31;
        return frame;
    }
    
    // Implement other MMeet-specific frame generators...
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
                generator_ = std::make_unique<UUgreenFrameGenerator>();
                break;
            case ProtocolType::MMeet:
                generator_ = std::make_unique<MMeetFrameGenerator>();
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
                generator_ = std::make_unique<UUgreenFrameGenerator>();
                break;
            case ProtocolType::MMeet:
                generator_ = std::make_unique<MMeetFrameGenerator>();
                break;
            // Add other protocols...
        }
    }
    
    /**
     * @brief Generate voltage request frame
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame requestVoltage(uint8_t module_address) {
        return generator_->generateVoltageRequest(module_address);
    }
    
    /**
     * @brief Generate current request frame
     * @param module_address Device address
     * @return Generated CAN frame
     */
    can_frame requestCurrent(uint8_t module_address) {
        return generator_->generateCurrentRequest(module_address);
    }
    
    /**
     * @brief Generate voltage set frame
     * @param module_address Device address
     * @param voltage Voltage value (in 0.1V units)
     * @return Generated CAN frame
     */
    can_frame setVoltage(uint8_t module_address, uint16_t voltage) {
        return generator_->generateVoltageSet(module_address, voltage);
    }
    
private:
    std::unique_ptr<ICanFrameGenerator> generator_;
};
