#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>

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
class PoweredDevice : public SmartDevice {
protected:
    double powerConsumption; 
    time_t lastTurnOnTime;
    time_t totalOnTime; 
    static double totalEnergyConsumed;
    
public:
    PoweredDevice(const std::string& id, const std::string& name, double power)
        : SmartDevice(id, name), powerConsumption(power), 
          lastTurnOnTime(0), totalOnTime(0) {}
    
    virtual ~PoweredDevice() = default;
    
    virtual void turnOn() override {
        if (!isOn) {
            isOn = true;
            lastTurnOnTime = time(nullptr);
        }
    }
    
    virtual void turnOff() override {
        if (isOn) {
            isOn = false;
            time_t currentTime = time(nullptr);
            time_t onDuration = currentTime - lastTurnOnTime;
            totalOnTime += onDuration;
            
            double energy = (powerConsumption * onDuration) / 3600000.0; 
            totalEnergyConsumed += energy;
        }
    }
    

    virtual double getPowerUsage() const {
        return isOn ? powerConsumption : 0;
    }
    
    virtual std::string getStatus() const override = 0;
    
    double getTotalOnTime() const { return totalOnTime; }
    
    static double getTotalEnergyConsumed() {
        return totalEnergyConsumed;
    }
    
    static void resetEnergyConsumption() {
        totalEnergyConsumed = 0;
    }
};

double PoweredDevice::totalEnergyConsumed = 0.0;