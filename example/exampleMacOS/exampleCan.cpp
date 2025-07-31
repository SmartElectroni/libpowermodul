#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include "../../libmodul.h" 
#include "ControlCAN.h" // Official CANalyst-II driver header

using namespace std;
using namespace std::chrono;

// ========== CONFIGURATION ==========
const ProtocolType PROTOCOL = ProtocolType::UUgreen; // Change to test different protocols
const uint8_t DEVICE_ADDR = 0x01;                   // CAN device address to test
const int CANALYST_DEV_TYPE = 4;                    // VCI_USBCAN2 device type
const int CANALYST_DEV_INDEX = 0;                   // First CANalyst device
const int CANALYST_CHANNEL = 0;                     // CAN channel (0 or 1)
const int CAN_BAUD_RATE = 0x031C;                   // 500kbps: 0x00 0x1C (0x001C)
                                                    // 250kbps: 0x01 0x1C (0x011C)
                                                    // 125kbps: 0x03 0x1C (0x031C)
const int RESPONSE_TIMEOUT_MS = 1000;               // Default response timeout
// ===================================

/**
 * @brief Wrapper class for CANalyst-II adapter
 * 
 * Handles device initialization, message sending/receiving
 */
class CANalystAdapter {
    bool initialized = false;
    
public:
    /**
     * @brief Initialize CANalyst-II device and CAN channel
     * @return true if initialization succeeded
     */
    bool init() {
        // 1. Open device
        if(VCI_OpenDevice(CANALYST_DEV_TYPE, CANALYST_DEV_INDEX, 0) != STATUS_OK) {
            cerr << "Error: Failed to open CANalyst device" << endl;
            return false;
        }

        // 2. Configure CAN channel
        VCI_INIT_CONFIG config;
        config.AccCode = 0x00000000;  // Accept all messages
        config.AccMask = 0xFFFFFFFF;  // Full mask
        config.Filter = 1;            // Receive all frames
        config.Mode = 0;              // Normal mode
        config.Timing0 = CAN_BAUD_RATE & 0xFF;
        config.Timing1 = (CAN_BAUD_RATE >> 8) & 0xFF;

        if(VCI_InitCAN(CANALYST_DEV_TYPE, CANALYST_DEV_INDEX, CANALYST_CHANNEL, &config) != STATUS_OK) {
            cerr << "Error: CAN channel initialization failed" << endl;
            return false;
        }

        // 3. Start CAN channel
        if(VCI_StartCAN(CANALYST_DEV_TYPE, CANALYST_DEV_INDEX, CANALYST_CHANNEL) != STATUS_OK) {
            cerr << "Error: Failed to start CAN channel" << endl;
            return false;
        }

        initialized = true;
        cout << "CANalyst-II initialized successfully" << endl;
        return true;
    }

    /**
     * @brief Send CAN frame
     * @param frame CAN frame to send
     * @return true if frame was sent successfully
     */
    bool send(const can_frame& frame) {
        VCI_CAN_OBJ msg;
        msg.ID = frame.can_id;
        msg.SendType = 0;             // Normal send (not single-shot)
        msg.RemoteFlag = 0;           // Data frame (not remote)
        msg.ExternFlag = frame.can_id > 0x7FF ? 1 : 0; // Extended ID if > 11-bit
        msg.DataLen = frame.can_dlc;
        memcpy(msg.Data, frame.data, frame.can_dlc);

        DWORD result = VCI_Transmit(CANALYST_DEV_TYPE, CANALYST_DEV_INDEX, 
                                   CANALYST_CHANNEL, &msg, 1);
        if(result != 1) {
            cerr << "Error: Failed to send CAN frame" << endl;
            return false;
        }
        return true;
    }

    /**
     * @brief Receive CAN frame with timeout
     * @param frame Reference to store received frame
     * @param timeout_ms Timeout in milliseconds
     * @return true if frame was received
     */
    bool receive(can_frame& frame, int timeout_ms) {
        VCI_CAN_OBJ msg;
        auto start = steady_clock::now();
        
        while(duration_cast<milliseconds>(steady_clock::now() - start).count() < timeout_ms) {
            DWORD count = VCI_Receive(CANALYST_DEV_TYPE, CANALYST_DEV_INDEX, 
                                     CANALYST_CHANNEL, &msg, 1, 10);
            if(count > 0) {
                frame.can_id = msg.ID;
                frame.can_dlc = msg.DataLen;
                memcpy(frame.data, msg.Data, msg.DataLen);
                return true;
            }
            this_thread::sleep_for(1ms);
        }
        return false;
    }

    ~CANalystAdapter() {
        if(initialized) {
            VCI_CloseDevice(CANALYST_DEV_TYPE, CANALYST_DEV_INDEX);
            cout << "CANalyst device closed" << endl;
        }
    }
};

/**
 * @brief Print CAN frame in human-readable format
 * @param frame CAN frame to print
 */
void printFrame(const can_frame& frame) {
    cout << "ID: 0x" << hex << setw(8) << setfill('0') << frame.can_id << dec << " ";
    cout << "DLC: " << static_cast<int>(frame.can_dlc) << " ";
    cout << "Data: ";
    for(int i = 0; i < frame.can_dlc; i++) {
        cout << hex << setw(2) << setfill('0') 
             << static_cast<int>(frame.data[i]) << " ";
    }
    cout << dec << endl;
}

/**
 * @brief Test complete protocol functionality
 * @param can CAN adapter instance
 * @param protocol Protocol type to test
 */
void testProtocol(CANalystAdapter& can, ProtocolType protocol) {
    CanProtocolManager manager(protocol);
    CanParser parser;
    
    cout << "\n=== Testing " 
         << (protocol == ProtocolType::UUgreen ? "UUgreen" : "MMeet") 
         << " protocol ===\n";

    // 1. Test voltage measurement
    cout << "\n[1] Testing voltage measurement..." << endl;
    can_frame frame = manager.generateVoltageRequest(DEVICE_ADDR);
    cout << "Sending voltage request: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        can_frame response;
        if(can.receive(response, RESPONSE_TIMEOUT_MS)) {
            cout << "Received response: ";
            printFrame(response);
            
            auto [data, result] = parser.parse(response, protocol);
            if(result == ParseResult::OK && data && data->fields.test(ParsedData::VOLTAGE)) {
                cout << "Measured voltage: " << data->voltage << " V" << endl;
            } else {
                cout << "Failed to parse voltage response" << endl;
            }
        } else {
            cout << "No response to voltage request" << endl;
        }
    }

    // 2. Test current measurement
    cout << "\n[2] Testing current measurement..." << endl;
    frame = manager.generateCurrentRequest(DEVICE_ADDR);
    cout << "Sending current request: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        can_frame response;
        if(can.receive(response, RESPONSE_TIMEOUT_MS)) {
            cout << "Received response: ";
            printFrame(response);
            
            auto [data, result] = parser.parse(response, protocol);
            if(result == ParseResult::OK && data && data->fields.test(ParsedData::CURRENT)) {
                cout << "Measured current: " << data->current << " A" << endl;
            } else {
                cout << "Failed to parse current response" << endl;
            }
        } else {
            cout << "No response to current request" << endl;
        }
    }

    // 3. Test temperature measurement
    cout << "\n[3] Testing temperature measurement..." << endl;
    frame = manager.generateTempRequest(DEVICE_ADDR);
    cout << "Sending temperature request: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        can_frame response;
        if(can.receive(response, RESPONSE_TIMEOUT_MS)) {
            cout << "Received response: ";
            printFrame(response);
            
            auto [data, result] = parser.parse(response, protocol);
            if(result == ParseResult::OK && data && data->fields.test(ParsedData::TEMP)) {
                cout << "Measured temperature: " << data->temperature << " °C" << endl;
            } else {
                cout << "Failed to parse temperature response" << endl;
            }
        } else {
            cout << "No response to temperature request" << endl;
        }
    }

    // 4. Test device status flags
    cout << "\n[4] Testing status flags..." << endl;
    frame = manager.generateFlagsRequest(DEVICE_ADDR);
    cout << "Sending flags request: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        can_frame response;
        if(can.receive(response, RESPONSE_TIMEOUT_MS)) {
            cout << "Received response: ";
            printFrame(response);
            
            auto [data, result] = parser.parse(response, protocol);
            if(result == ParseResult::OK && data && data->fields.test(ParsedData::STATUS)) {
                cout << "Device status: 0x" << hex << data->status << dec << endl;
            } else {
                cout << "Failed to parse status response" << endl;
            }
        } else {
            cout << "No response to flags request" << endl;
        }
    }

    // 5. Test current capability
    cout << "\n[5] Testing current capability..." << endl;
    frame = manager.generateCurrentCapabilityRequest(DEVICE_ADDR);
    cout << "Sending capability request: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        can_frame response;
        if(can.receive(response, RESPONSE_TIMEOUT_MS)) {
            cout << "Received response: ";
            printFrame(response);
            
            auto [data, result] = parser.parse(response, protocol);
            if(result == ParseResult::OK && data && data->fields.test(ParsedData::CAPABILITY)) {
                cout << "Current capability: " << data->current_capability << " A" << endl;
            } else {
                cout << "Failed to parse capability response" << endl;
            }
        } else {
            cout << "No response to capability request" << endl;
        }
    }

    // 6. Test voltage setting
    const float test_voltage = 350.5f;
    cout << "\n[6] Testing voltage set to " << test_voltage << "V..." << endl;
    frame = manager.generateVoltageSet(DEVICE_ADDR, test_voltage);
    cout << "Sending voltage set: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        cout << "Voltage set command sent" << endl;
    } else {
        cout << "Failed to send voltage set" << endl;
    }

    // 7. Test current setting
    const float test_current = 10.5f;
    cout << "\n[7] Testing current set to " << test_current << "A..." << endl;
    frame = manager.generateCurrentSet(DEVICE_ADDR, test_current);
    cout << "Sending current set: ";
    printFrame(frame);
    
    if(can.send(frame)) {
        cout << "Current set command sent" << endl;
    } else {
        cout << "Failed to send current set" << endl;
    }

    // 8. Test operation modes
    cout << "\n[8] Testing operation modes..." << endl;
    
    cout << "Setting low mode..." << endl;
    frame = manager.generateLowModeSet(DEVICE_ADDR);
    can.send(frame);
    this_thread::sleep_for(500ms);
    
    cout << "Setting high mode..." << endl;
    frame = manager.generateHighModeSet(DEVICE_ADDR);
    can.send(frame);
    this_thread::sleep_for(500ms);
    
    cout << "Setting auto mode..." << endl;
    if(auto auto_frame = manager.generateAutoModeSet(DEVICE_ADDR)) {
        can.send(*auto_frame);
    } else {
        cout << "Auto mode not supported by this protocol" << endl;
    }
    this_thread::sleep_for(500ms);

    // 9. Test enable/disable
    cout << "\n[9] Testing device enable/disable..." << endl;
    
    cout << "Enabling device..." << endl;
    frame = manager.generateEnable(DEVICE_ADDR);
    can.send(frame);
    this_thread::sleep_for(1s);
    
    cout << "Disabling device..." << endl;
    frame = manager.generateDisable(DEVICE_ADDR);
    can.send(frame);
    this_thread::sleep_for(1s);
}

int main() {
    cout << "CAN Protocol Tester for CANalyst-II" << endl;
    cout << "Protocol: " << (PROTOCOL == ProtocolType::UUgreen ? "UUgreen" : "MMeet") << endl;
    cout << "Device address: 0x" << hex << static_cast<int>(DEVICE_ADDR) << dec << endl;

    try {
        // Initialize CAN adapter
        CANalystAdapter can;
        if(!can.init()) {
            cerr << "Fatal: CAN adapter initialization failed" << endl;
            return 1;
        }

        // Run complete protocol test
        testProtocol(can, PROTOCOL);

        cout << "\n=== All tests completed ===" << endl;
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}