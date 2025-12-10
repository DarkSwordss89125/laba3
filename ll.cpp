#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

// Интерфейс для датчиков
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual double getCurrentVoltage() const = 0;
    virtual std::string getSensorType() const = 0;
};

// Абстрактный базовый класс SmartDevice
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
    
    // Чисто виртуальные функции
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual std::string getStatus() const = 0;
    
    // Виртуальная функция с реализацией по умолчанию
    virtual std::string getDeviceInfo() const {
        return "Device: " + deviceName + " (ID: " + deviceId + ")";
    }
    
    bool getIsOn() const { return isOn; }
    std::string getId() const { return deviceId; }
    std::string getName() const { return deviceName; }
    
    // Статический член для подсчета созданных устройств
    static int totalDevicesCreated;
    
    // Статическая функция
    static int getTotalDevicesCreated() {
        return totalDevicesCreated;
    }
};

// Инициализация статического члена
int SmartDevice::totalDevicesCreated = 0;

// Промежуточный класс PoweredDevice
class PoweredDevice : public SmartDevice {
protected:
    double powerConsumption; // в ваттах
    time_t lastTurnOnTime;
    time_t totalOnTime; // в секундах
    
    // Статический член для общего потребления энергии
    static double totalEnergyConsumed; // в кВт·ч
    
public:
    PoweredDevice(const std::string& id, const std::string& name, double power)
        : SmartDevice(id, name), powerConsumption(power), 
          lastTurnOnTime(0), totalOnTime(0) {}
    
    virtual ~PoweredDevice() = default;
    
    // Переопределение чисто виртуальных функций
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
            
            // Рассчитываем потребленную энергию
            double energy = (powerConsumption * onDuration) / 3600000.0; // кВт·ч
            totalEnergyConsumed += energy;
        }
    }
    
    // Виртуальная функция для получения информации о потреблении
    virtual double getPowerUsage() const {
        return isOn ? powerConsumption : 0;
    }
    
    // Чисто виртуальная функция из SmartDevice
    virtual std::string getStatus() const override = 0;
    
    double getTotalOnTime() const { return totalOnTime; }
    
    // Статическая функция для получения общего потребления
    static double getTotalEnergyConsumed() {
        return totalEnergyConsumed;
    }
    
    // Статическая функция для сброса статистики
    static void resetEnergyConsumption() {
        totalEnergyConsumed = 0;
    }
};

// Инициализация статического члена
double PoweredDevice::totalEnergyConsumed = 0.0;

// Класс LightBulb
class LightBulb : public PoweredDevice {
private:
    int brightness; // 0-100%
    std::string color;
    
public:
    LightBulb(const std::string& id, const std::string& name, 
              double power, int brightness = 100, const std::string& color = "white")
        : PoweredDevice(id, name, power), brightness(brightness), color(color) {
        if (brightness < 0 || brightness > 100) {
            throw std::invalid_argument("Brightness must be between 0 and 100");
        }
    }
    
    // Конструктор копирования
    LightBulb(const LightBulb& other)
        : PoweredDevice(other), brightness(other.brightness), color(other.color) {}
    
    // Оператор присваивания
    LightBulb& operator=(const LightBulb& other) {
        if (this != &other) {
            PoweredDevice::operator=(other);
            brightness = other.brightness;
            color = other.color;
        }
        return *this;
    }
    
    // Переопределение виртуальных функций
    virtual std::string getStatus() const override {
        std::stringstream ss;
        ss << "LightBulb " << deviceName << " is " << (isOn ? "ON" : "OFF")
           << ", Brightness: " << brightness << "%, Color: " << color
           << ", Power: " << powerConsumption << "W";
        return ss.str();
    }
    
    // Переопределение функции с реализацией по умолчанию
    virtual std::string getDeviceInfo() const override {
        return PoweredDevice::getDeviceInfo() + " [Light Bulb]";
    }
    
    void setBrightness(int level) {
        if (level >= 0 && level <= 100) {
            brightness = level;
        }
    }
    
    void setColor(const std::string& newColor) {
        color = newColor;
    }
    
    int getBrightness() const { return brightness; }
    std::string getColor() const { return color; }
};

// Класс Thermostat
class Thermostat : public PoweredDevice {
private:
    double currentTemperature;
    double targetTemperature;
    std::string mode; // "heating", "cooling", "off"
    
public:
    Thermostat(const std::string& id, const std::string& name, 
               double power, double initialTemp = 20.0)
        : PoweredDevice(id, name, power), currentTemperature(initialTemp),
          targetTemperature(initialTemp), mode("off") {}
    
    // Конструктор копирования
    Thermostat(const Thermostat& other)
        : PoweredDevice(other), currentTemperature(other.currentTemperature),
          targetTemperature(other.targetTemperature), mode(other.mode) {}
    
    // Оператор присваивания
    Thermostat& operator=(const Thermostat& other) {
        if (this != &other) {
            PoweredDevice::operator=(other);
            currentTemperature = other.currentTemperature;
            targetTemperature = other.targetTemperature;
            mode = other.mode;
        }
        return *this;
    }
    
    virtual void turnOn() override {
        PoweredDevice::turnOn();
        if (mode == "off") {
            mode = "heating";
        }
    }
    
    virtual void turnOff() override {
        PoweredDevice::turnOff();
        mode = "off";
    }
    
    virtual std::string getStatus() const override {
        std::stringstream ss;
        ss << "Thermostat " << deviceName << " is " << (isOn ? "ON" : "OFF")
           << ", Current: " << std::fixed << std::setprecision(1) << currentTemperature
           << "°C, Target: " << targetTemperature << "°C, Mode: " << mode
           << ", Power: " << powerConsumption << "W";
        return ss.str();
    }
    

    virtual std::string getDeviceInfo() const override {
        return PoweredDevice::getDeviceInfo() + " [Thermostat]";
    }
    
    virtual double getPowerUsage() const override {
        if (!isOn || mode == "off") {
            return 0;
        }
        double tempDiff = std::abs(targetTemperature - currentTemperature);
        return powerConsumption * (0.5 + tempDiff / 10.0);
    }
    
    void setTargetTemperature(double temp) {
        targetTemperature = temp;
        if (!isOn && temp != currentTemperature) {
            turnOn();
        }
    }
    
    void updateTemperature(double newTemp) {
        currentTemperature = newTemp;
    }
    
    void setMode(const std::string& newMode) {
        mode = newMode;
        if (newMode != "off" && !isOn) {
            turnOn();
        }
    }
    
    double getCurrentTemperature() const { return currentTemperature; }
    double getTargetTemperature() const { return targetTemperature; }
    std::string getMode() const { return mode; }
};


class SmartOutlet : public PoweredDevice, public ISensor {
private:
    bool outletOn;
    double voltage;
    double maxCurrent;
    std::string location;
    mutable int sensorCounter;
    
public:
    SmartOutlet(const std::string& id, const std::string& name, 
                double power, double maxCurrent = 16.0, const std::string& location = "living room")
        : PoweredDevice(id, name, power), outletOn(false), 
          voltage(220.0), maxCurrent(maxCurrent), location(location),
          sensorCounter(0) {}
    
    SmartOutlet(const SmartOutlet& other)
        : PoweredDevice(other), ISensor(other),
          outletOn(other.outletOn), voltage(other.voltage),
          maxCurrent(other.maxCurrent), location(other.location),
          sensorCounter(other.sensorCounter) {}
    
    SmartOutlet& operator=(const SmartOutlet& other) {
        if (this != &other) {
            PoweredDevice::operator=(other);
            outletOn = other.outletOn;
            voltage = other.voltage;
            maxCurrent = other.maxCurrent;
            location = other.location;
            sensorCounter = other.sensorCounter;
        }
        return *this;
    }
    
    virtual double getCurrentVoltage() const override {
        sensorCounter++;
        return voltage + (std::sin(sensorCounter * 0.1) * 2.0) + ((std::rand() % 100) / 100.0);
    }
    
    virtual std::string getSensorType() const override {
        return "Voltage Sensor";
    }
    
    virtual void turnOn() override {
        PoweredDevice::turnOn();
        outletOn = true;
    }
    
    virtual void turnOff() override {
        PoweredDevice::turnOff();
        outletOn = false;
    }
    
    virtual std::string getStatus() const override {
        std::stringstream ss;
        ss << "SmartOutlet " << deviceName << " is " << (isOn ? "ON" : "OFF")
           << ", Outlet: " << (outletOn ? "active" : "inactive")
           << ", Location: " << location
           << ", Max Current: " << maxCurrent << "A"
           << ", Power: " << powerConsumption << "W";
        return ss.str();
    }
    
    virtual std::string getDeviceInfo() const override {
        return PoweredDevice::getDeviceInfo() + " [Smart Outlet with Sensor]";
    }
    virtual double getPowerUsage() const override {
        if (!isOn || !outletOn) {
            return 0;
        }
        return powerConsumption;
    }
    
    // Специфичные методы для SmartOutlet
    void toggleOutlet() {
        outletOn = !outletOn;
    }
    
    bool isOutletOn() const { return outletOn; }
    double getMaxCurrent() const { return maxCurrent; }
    std::string getLocation() const { return location; }
};