
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "../libmodul.h" 

using namespace std;

// ========== Configuration test ==========
const ProtocolType CURRENT_PROTOCOL = ProtocolType::UUgreen;
const uint8_t DEVICE_ADDRESS = 1;
const string CAN_INTERFACE = "can0";
const float TEST_VOLTAGE = 350.0f;
const float TEST_CURRENT = 10.5f;
const int RESPONSE_TIMEOUT_MS = 1000;
// ========================================

int open_can_socket(const string& interface) {
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        throw runtime_error("Error opening CAN socket");
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        close(s);
        throw runtime_error("Error getting CAN interface index");
    }

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(s);
        throw runtime_error("Error binding CAN socket");
    }

    return s;
}

void printCanFrame(const can_frame& frame) {
    cout << "CAN ID: 0x" << hex << frame.can_id << dec 
         << ", DLC: " << static_cast<int>(frame.can_dlc) 
         << ", Data: ";
    for (int i = 0; i < frame.can_dlc; ++i) {
        cout << hex << static_cast<int>(frame.data[i]) << " ";
    }
    cout << dec << endl;
}

void printParsedData(const ParsedData& data) {
    cout << "Parsed Data:" << endl;
    if (data.fields.test(ParsedData::ADDR))
        cout << "  Address: " << static_cast<int>(data.address) << endl;
    if (data.fields.test(ParsedData::VOLTAGE))
        cout << "  Voltage: " << data.voltage << " V" << endl;
    if (data.fields.test(ParsedData::CURRENT))
        cout << "  Current: " << data.current << " A" << endl;
    if (data.fields.test(ParsedData::TEMP))
        cout << "  Temperature: " << data.temperature << " °C" << endl;
    if (data.fields.test(ParsedData::STATUS))
        cout << "  Status: 0x" << hex << data.status << dec << endl;
    if (data.fields.test(ParsedData::CAPABILITY))
        cout << "  Current capability: " << data.current_capability << " A" << endl;
}

bool sendCanFrame(int socket, const can_frame& frame) {
    if (write(socket, &frame, sizeof(frame)) != sizeof(frame)) {
        cerr << "Failed to send CAN frame" << endl;
        return false;
    }
    return true;
}

optional<can_frame> receiveCanFrame(int socket, int timeout_ms) {
    can_frame frame;
    fd_set read_fds;
    struct timeval timeout;
    
    FD_ZERO(&read_fds);
    FD_SET(socket, &read_fds);
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    
    int ret = select(socket + 1, &read_fds, NULL, NULL, &timeout);
    if (ret > 0) {
        if (read(socket, &frame, sizeof(frame)) > 0) {
            return frame;
        }
    } else if (ret == 0) {
        cerr << "Timeout waiting for response" << endl;
    } else {
        cerr << "Error waiting for response" << endl;
    }
    return nullopt;
}

void test_protocol(int can_socket) {
    CanProtocolManager manager(CURRENT_PROTOCOL);
    CanParser parser;
    
    cout << "\n=== Testing protocol: " 
         << (CURRENT_PROTOCOL == ProtocolType::UUgreen ? "UUgreen" : "MMeet") 
         << " ===\n";

    // 1. Request and read voltage
    cout << "\n[1] Testing voltage request..." << endl;
    can_frame voltage_req = manager.generateVoltageRequest(DEVICE_ADDRESS);
    printCanFrame(voltage_req);
    
    if (sendCanFrame(can_socket, voltage_req)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 2. Request and read current
    cout << "\n[2] Testing current request..." << endl;
    can_frame current_req = manager.generateCurrentRequest(DEVICE_ADDRESS);
    printCanFrame(current_req);
    
    if (sendCanFrame(can_socket, current_req)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 3. Request and read temperature
    cout << "\n[3] Testing temperature request..." << endl;
    can_frame temp_req = manager.generateTempRequest(DEVICE_ADDRESS);
    printCanFrame(temp_req);
    
    if (sendCanFrame(can_socket, temp_req)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 4. Request and read flags state
    cout << "\n[4] Testing flags request..." << endl;
    can_frame flags_req = manager.generateFlagsRequest(DEVICE_ADDRESS);
    printCanFrame(flags_req);
    
    if (sendCanFrame(can_socket, flags_req)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 5. Request and read maximal capability current
    cout << "\n[5] Testing current capability request..." << endl;
    can_frame cap_req = manager.generateCurrentCapabilityRequest(DEVICE_ADDRESS);
    printCanFrame(cap_req);
    
    if (sendCanFrame(can_socket, cap_req)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 6. Set voltage
    cout << "\n[6] Testing voltage set to " << TEST_VOLTAGE << "V..." << endl;
    can_frame set_voltage = manager.generateVoltageSet(DEVICE_ADDRESS, TEST_VOLTAGE);
    printCanFrame(set_voltage);
    
    if (sendCanFrame(can_socket, set_voltage)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 7. Set current
    cout << "\n[7] Testing current set to " << TEST_CURRENT << "A..." << endl;
    can_frame set_current = manager.generateCurrentSet(DEVICE_ADDRESS, TEST_CURRENT);
    printCanFrame(set_current);
    
    if (sendCanFrame(can_socket, set_current)) {
        if (auto response = receiveCanFrame(can_socket, RESPONSE_TIMEOUT_MS)) {
            printCanFrame(*response);
            auto [parsed_data, parse_result] = parser.parse(*response, CURRENT_PROTOCOL);
            if (parse_result == ParseResult::OK && parsed_data) {
                printParsedData(*parsed_data);
            }
        }
    }

    // 8. Testing modes
    cout << "\n[8] Testing modes..." << endl;
    
    // Set low mode
    cout << "Setting low mode..." << endl;
    can_frame low_mode = manager.generateLowModeSet(DEVICE_ADDRESS);
    printCanFrame(low_mode);
    sendCanFrame(can_socket, low_mode);
    sleep(1);
    
    // УSet high mode
    cout << "Setting high mode..." << endl;
    can_frame high_mode = manager.generateHighModeSet(DEVICE_ADDRESS);
    printCanFrame(high_mode);
    sendCanFrame(can_socket, high_mode);
    sleep(1);
    
    // Set auto mode(if support)
    cout << "Setting auto mode..." << endl;
    if (auto auto_mode = manager.generateAutoModeSet(DEVICE_ADDRESS)) {
        printCanFrame(*auto_mode);
        sendCanFrame(can_socket, *auto_mode);
    } else {
        cout << "Auto mode not supported by this protocol" << endl;
    }
    sleep(1);

    // 9. Testing enable/disable
    cout << "\n[9] Testing enable/disable..." << endl;
    
    // Enabling device
    cout << "Enabling device..." << endl;
    can_frame enable = manager.generateEnable(DEVICE_ADDRESS);
    printCanFrame(enable);
    sendCanFrame(can_socket, enable);
    sleep(1);
    
    // Disabling device
    cout << "Disabling device..." << endl;
    can_frame disable = manager.generateDisable(DEVICE_ADDRESS);
    printCanFrame(disable);
    sendCanFrame(can_socket, disable);
    sleep(1);
}

int main() {
    int can_socket = -1;
    
    try {
        cout << "Starting CAN protocol tester" << endl;
        cout << "Using protocol: ";
        switch(CURRENT_PROTOCOL) {
            case ProtocolType::UUgreen: cout << "UUgreen"; break;
            case ProtocolType::MMeet: cout << "MMeet"; break;
            default: cout << "Unknown"; break;
        }
        cout << endl;
        
        cout << "Device address: " << static_cast<int>(DEVICE_ADDRESS) << endl;
        cout << "CAN interface: " << CAN_INTERFACE << endl;
        
        can_socket = open_can_socket(CAN_INTERFACE);
        cout << "CAN interface opened successfully" << endl;
        
        test_protocol(can_socket);
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        if (can_socket >= 0) close(can_socket);
        return 1;
    }
    
    if (can_socket >= 0) close(can_socket);
    cout << "\nTesting completed successfully" << endl;
    return 0;
}