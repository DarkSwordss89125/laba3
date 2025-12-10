#include <iostream>
#include <string>
#include <stdexcept>

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual double getCurrentVoltage() const = 0;
    virtual std::string getSensorType() const = 0;
};

class SmartDevice {
protected:
    std::string deviceId;
    std::string deviceName;
    bool isOn;
    
public:
    SmartDevice(const std::string& id, const std::string& name)
        : deviceId(id), deviceName(name), isOn(false) {
        totalDevicesCreated++;
    }
    
    virtual ~SmartDevice() = default;
    

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual std::string getStatus() const = 0;
    

    virtual std::string getDeviceInfo() const {
        return "Device: " + deviceName + " (ID: " + deviceId + ")";
    }
    
    bool getIsOn() const { return isOn; }
    std::string getId() const { return deviceId; }
    std::string getName() const { return deviceName; }

    static int totalDevicesCreated;
    

    static int getTotalDevicesCreated() {
        return totalDevicesCreated;
    }
};

int SmartDevice::totalDevicesCreated = 0;