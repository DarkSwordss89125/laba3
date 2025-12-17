#ifndef SMART_DEVICES_HPP
#define SMART_DEVICES_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

// Интерфейс для датчика
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual double getCurrentVoltage() const = 0;
    virtual std::string getSensorType() const = 0;
};

// Абстрактный базовый класс для всех умных устройств
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
    
    SmartDevice(const SmartDevice& other)
        : deviceId(other.deviceId + "_copy"), 
          deviceName(other.deviceName + " (copy)"), 
          isOn(other.isOn) {
        totalDevicesCreated++;
    }
    
    virtual ~SmartDevice() = default;
    
    SmartDevice& operator=(const SmartDevice& other) {
        if (this != &other) {
            deviceId = other.deviceId + "_assigned";
            deviceName = other.deviceName + " (assigned)";
            isOn = other.isOn;
        }
        return *this;
    }
    
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual std::string getStatus() const = 0;
    
    virtual std::string getDeviceInfo() const {
        return "Ustroystvo: " + deviceName + " (ID: " + deviceId + ")";
    }
    
    bool getIsOn() const { return isOn; }
    std::string getId() const { return deviceId; }
    std::string getName() const { return deviceName; }
    
    static int totalDevicesCreated;
    static int getTotalDevicesCreated();
};

// Промежуточный класс для устройств с питанием
class PoweredDevice : public SmartDevice {
protected:
    double powerConsumption;
    time_t lastTurnOnTime;
    time_t totalOnTime;
    
    static double totalEnergyConsumedAll;
    
public:
    PoweredDevice(const std::string& id, const std::string& name, double power);
    
    PoweredDevice(const PoweredDevice& other);
    
    virtual ~PoweredDevice() = default;
    
    PoweredDevice& operator=(const PoweredDevice& other);
    
    virtual void turnOn() override;
    virtual void turnOff() override;
    
    virtual double getPowerUsage() const;
    virtual std::string getStatus() const override = 0;
    
    double getTotalOnTime() const;
    double getCurrentSessionTime() const;
    double getDeviceEnergyConsumed() const;
    
    static double getTotalEnergyConsumedAll();
    static void resetEnergyConsumption();
    
    std::string getFormattedOnTime() const;
    double getOnTimeInHours() const;
    double getPowerConsumption() const;
};

// Конкретный класс - Лампочка
class LightBulb : public PoweredDevice {
private:
    int brightness;
    std::string color;
    
public:
    LightBulb(const std::string& id, const std::string& name, 
              double power, int brightness = 100, const std::string& color = "teplyy belyy");
    
    LightBulb(const LightBulb& other);
    
    LightBulb& operator=(const LightBulb& other);
    
    virtual std::string getStatus() const override;
    virtual std::string getDeviceInfo() const override;
    
    void setBrightness(int level);
    void setColor(const std::string& newColor);
    
    int getBrightness() const;
    std::string getColor() const;
    
    void displayInfo() const;
};

// Конкретный класс - Термостат
class Thermostat : public PoweredDevice {
private:
    double currentTemperature;
    double targetTemperature;
    std::string mode;
    
public:
    Thermostat(const std::string& id, const std::string& name, 
               double power, double initialTemp = 20.0);
    
    Thermostat(const Thermostat& other);
    
    Thermostat& operator=(const Thermostat& other);
    
    virtual void turnOn() override;
    virtual void turnOff() override;
    
    virtual std::string getStatus() const override;
    virtual std::string getDeviceInfo() const override;
    virtual double getPowerUsage() const override;
    
    void setTargetTemperature(double temp);
    void updateTemperature(double newTemp);
    void setMode(const std::string& newMode);
    
    double getCurrentTemperature() const;
    double getTargetTemperature() const;
    std::string getMode() const;
    
    void displayInfo() const;
};

// Класс с множественным наследованием - Умная розетка
class SmartOutlet : public PoweredDevice, public ISensor {
private:
    bool outletOn;
    double voltage;
    double maxCurrent;
    
public:
    SmartOutlet(const std::string& id, const std::string& name, 
                double power, double maxCurrent = 16.0);
    
    SmartOutlet(const SmartOutlet& other);
    
    SmartOutlet& operator=(const SmartOutlet& other);
    
    virtual double getCurrentVoltage() const override;
    virtual std::string getSensorType() const override;
    
    virtual void turnOn() override;
    virtual void turnOff() override;
    
    virtual std::string getStatus() const override;
    virtual std::string getDeviceInfo() const override;
    virtual double getPowerUsage() const override;
    
    void toggleOutlet();
    
    bool isOutletOn() const;
    double getMaxCurrent() const;
    double getVoltage() const;
    
    void displayInfo() const;
};

#endif