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
    can_frame generateAutoModeSet(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in 0.1V units)
     * @return Generated CAN frame
     */
    can_frame generateVoltageSet(uint8_t module_address, uint16_t voltage) override;
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in 0.1A units)
     * @return Generated CAN frame
     */
    can_frame generateCurrentSet(uint8_t module_address, uint16_t current) override;

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
    can_frame init_frame (uint8_t module_address) override;    
    
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
    can_frame generateAutoModeSet(uint8_t module_address) override;

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in 0.1V units)
     * @return Generated CAN frame
     */
    can_frame generateVoltageSet(uint8_t module_address, uint16_t voltage) override;
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in 0.1A units)
     * @return Generated CAN frame
     */
    can_frame generateCurrentSet(uint8_t module_address, uint16_t current) override;

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
    can_frame init_frame (uint8_t module_address) override;    
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
    can_frame generateAutoModeSet(uint8_t module_address) {
        return _generator->generateAutoModeSet(module_address);
    }

    /**
     * @brief Generate CAN frame for voltage setting
     * @param module_address Device address
     * @param voltage Voltage value (in 0.1V units)
     * @return Generated CAN frame
     */
    can_frame generateVoltageSet(uint8_t module_address, uint16_t voltage) {
        return _generator->generateVoltageSet(module_address , voltage);
    }
    
    /**
     * @brief Generate CAN frame for current setting
     * @param module_address Device address
     * @param current Current value (in 0.1A units)
     * @return Generated CAN frame
     */
    can_frame generateCurrentSet(uint8_t module_address, uint16_t current) {
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
