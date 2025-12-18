/**
 * @file smart_devices.hpp
 * @brief Библиотека для работы с умными устройствами системы "Умный дом"
 * @author Elmira
 * @date 2025 декабрь
 * @version 1.0
 * 
 * @details
 * Данная библиотека реализует иерархию классов для управления умными устройствами:
 * - Абстрактные интерфейсы и базовые классы
 * - Устройства с питанием (лампочки, термостаты)
 * - Устройства с множественным наследованием (умные розетки)
 * 
 * Предоставляет функциональность для:
 * - Управления состояниями устройств (включение/выключение)
 * - Мониторинга потребления энергии
 * - Сбора статистики использования
 * - Работы с датчиками и сенсорами
 * 
 * Использует механизмы наследования, полиморфизма и статических членов классов.
 */

#ifndef SMART_DEVICES_HPP
#define SMART_DEVICES_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>

/**
 * @class ISensor
 * @brief Интерфейс для устройств с функциями датчиков
 * 
 * @details
 * Определяет общий интерфейс для получения данных с датчиков.
 * Используется для реализации множественного наследования.
 * 
 * @note Все классы, реализующие этот интерфейс, должны предоставлять
 *       информацию о текущей мощности и типе датчика.
 */
class ISensor {
public:
    /**
     * @brief Виртуальный деструктор
     * @details Гарантирует корректное удаление объектов производных классов
     */
    virtual ~ISensor() = default;
    
    /**
     * @brief Получить текущую мощность
     * @return Текущая мощность в ваттах
     * @pure
     */
    virtual double getCurrentPower() const = 0;
    
    /**
     * @brief Получить тип датчика
     * @return Строка с описанием типа датчика
     * @pure
     */
    virtual std::string getSensorType() const = 0;
};

/**
 * @class SmartDevice
 * @brief Абстрактный базовый класс для всех умных устройств
 * 
 * @details
 * Определяет общий интерфейс и базовую функциональность для всех устройств:
 * - Идентификатор и имя устройства
 * - Состояние (включено/выключено)
 * - Статистика создания устройств
 */
class SmartDevice {
protected:
    std::string deviceId;       ///< Уникальный идентификатор устройства
    std::string deviceName;     ///< Имя устройства
    bool isOn;                  ///< Состояние устройства (true = включено)
    
public:
    /**
     * @brief Основной конструктор
     * @param id Уникальный идентификатор устройства
     * @param name Имя устройства для отображения
     * @post Увеличивает счетчик totalDevicesCreated на 1
     * @post Устанавливает isOn = false
     */
    SmartDevice(const std::string& id, const std::string& name)
        : deviceId(id), deviceName(name), isOn(false) {
        totalDevicesCreated++;
    }
    
    /**
     * @brief Копирующий конструктор
     * @param other Устройство для копирования
     * @post Создает копию с "_copy" в имени и ID
     * @post Увеличивает счетчик totalDevicesCreated на 1
     */
    SmartDevice(const SmartDevice& other)
        : deviceId(other.deviceId + "_copy"), 
          deviceName(other.deviceName + " (copy)"), 
          isOn(other.isOn) {
        totalDevicesCreated++;
    }
    
    /**
     * @brief Виртуальный деструктор
     * @details Гарантирует корректное удаление объектов производных классов
     */
    virtual ~SmartDevice() = default;
    
    /**
     * @brief Оператор присваивания
     * @param other Устройство для копирования
     * @return Ссылка на текущий объект
     * @post Добавляет "_assigned" к ID и имени
     */
    SmartDevice& operator=(const SmartDevice& other) {
        if (this != &other) {
            deviceId = other.deviceId + "_assigned";
            deviceName = other.deviceName + " (assigned)";
            isOn = other.isOn;
        }
        return *this;
    }
    
    /**
     * @brief Включить устройство
     * @pure
     */
    virtual void turnOn() = 0;
    
    /**
     * @brief Выключить устройство
     * @pure
     */
    virtual void turnOff() = 0;
    
    /**
     * @brief Получить текущий статус устройства
     * @return Строка с описанием статуса
     * @pure
     */
    virtual std::string getStatus() const = 0;
    
    /**
     * @brief Получить информацию об устройстве
     * @return Строка с основной информацией об устройстве
     * @details Формат: "Ustroystvo: [имя] (ID: [идентификатор])"
     */
    virtual std::string getDeviceInfo() const {
        return "Ustroystvo: " + deviceName + " (ID: " + deviceId + ")";
    }
    
    /**
     * @brief Получить текущую мощность устройства
     * @return Текущая мощность в ваттах
     * @details Базовая реализация возвращает 0
     */
    virtual double getCurrentPower() const {
        return 0.0;
    }
    
    /**
     * @brief Проверить, включено ли устройство
     * @return true если устройство включено, false в противном случае
     */
    bool getIsOn() const { return isOn; }
    
    /**
     * @brief Получить идентификатор устройства
     * @return Уникальный идентификатор устройства
     */
    std::string getId() const { return deviceId; }
    
    /**
     * @brief Получить имя устройства
     * @return Имя устройства для отображения
     */
    std::string getName() const { return deviceName; }
    
    /**
     * @brief Счетчик созданных устройств
     * @details Увеличивается при создании любого устройства
     */
    static int totalDevicesCreated;
    
    /**
     * @brief Получить общее количество созданных устройств
     * @return Количество созданных экземпляров SmartDevice
     */
    static int getTotalDevicesCreated();
};

// Инициализация статических переменных
int SmartDevice::totalDevicesCreated = 0;

int SmartDevice::getTotalDevicesCreated() {
    return totalDevicesCreated;
}

/**
 * @class PoweredDevice
 * @brief Промежуточный класс для устройств с учетом потребления энергии
 * 
 * @details
 * Наследует SmartDevice и добавляет функциональность для:
 * - Учета потребляемой мощности
 * - Отслеживания времени работы
 * - Сбора статистики по потреблению энергии
 * 
 * @note Используется как базовый класс для всех энергопотребляющих устройств
 */
class PoweredDevice : public SmartDevice {
protected:
    double powerConsumption;    ///< Потребляемая мощность (в ваттах)
    time_t lastTurnOnTime;      ///< Время последнего включения (timestamp)
    time_t totalOnTime;         ///< Общее время работы (в секундах)
    
    /**
     * @brief Общее потребление энергии всеми устройствами
     * @details Измеряется в ватт-часах
     */
    static double totalEnergyConsumedAll;
    
public:
    /**
     * @brief Конструктор устройства с питанием
     * @param id Уникальный идентификатор
     * @param name Имя устройства
     * @param power Потребляемая мощность в ваттах
     * @throws std::invalid_argument если power <= 0
     * @post Инициализирует счетчики времени
     */
    PoweredDevice(const std::string& id, const std::string& name, double power);
    
    /**
     * @brief Копирующий конструктор
     * @param other Устройство для копирования
     * @post Копирует все параметры кроме статистики времени
     */
    PoweredDevice(const PoweredDevice& other);
    
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~PoweredDevice() = default;
    
    /**
     * @brief Оператор присваивания
     * @param other Устройство для копирования
     * @return Ссылка на текущий объект
     */
    PoweredDevice& operator=(const PoweredDevice& other);
    
    /**
     * @brief Включить устройство
     * @override
     * @post Запоминает время включения
     */
    virtual void turnOn() override;
    
    /**
     * @brief Выключить устройство
     * @override
     * @post Обновляет общее время работы
     * @post Добавляет потребленную энергию в общую статистику
     */
    virtual void turnOff() override;
    
    /**
     * @brief Получить текущее энергопотребление
     * @return Потребляемая мощность в ваттах
     */
    virtual double getPowerUsage() const {
        return isOn ? powerConsumption : 0.0; // Используем реальную мощность устройства
    }
    
    /**
     * @brief Получить статус устройства
     * @return Строка с описанием статуса
     * @pure
     */
    virtual std::string getStatus() const override = 0;
    
    /**
     * @brief Получить общее время работы
     * @return Общее время работы в секундах
     */
    double getTotalOnTime() const;
    
    /**
     * @brief Получить время текущей сессии
     * @return Время текущей сессии в секундах
     * @details Возвращает 0 если устройство выключено
     */
    double getCurrentSessionTime() const;
    
    /**
     * @brief Получить общую потребленную энергию
     * @return Потребленная энергия в ватт-часах
     */
    double getDeviceEnergyConsumed() const;
    
    /**
     * @brief Получить общее потребление энергии всеми устройствами
     * @return Суммарная потребленная энергия в ватт-часах
     */
    static double getTotalEnergyConsumedAll();
    
    /**
     * @brief Сбросить статистику потребления энергии
     * @post totalEnergyConsumedAll = 0
     */
    static void resetEnergyConsumption();
    
    /**
     * @brief Получить отформатированное время работы
     * @return Строка в формате "ЧЧ:ММ:СС"
     */
    std::string getFormattedOnTime() const;
    
    /**
     * @brief Получить время работы в часах
     * @return Время работы в часах (с плавающей точкой)
     */
    double getOnTimeInHours() const;
    
    /**
     * @brief Получить мощность потребления
     * @return Потребляемая мощность в ваттах
     */
    double getPowerConsumption() const;
};

// Инициализация статических переменных PoweredDevice
double PoweredDevice::totalEnergyConsumedAll = 0.0;

// Реализация методов PoweredDevice
PoweredDevice::PoweredDevice(const std::string& id, const std::string& name, double power)
    : SmartDevice(id, name), powerConsumption(power), lastTurnOnTime(0), totalOnTime(0) {
    if (power <= 0) {
        throw std::invalid_argument("Power consumption must be positive");
    }
}

PoweredDevice::PoweredDevice(const PoweredDevice& other)
    : SmartDevice(other), 
      powerConsumption(other.powerConsumption),
      lastTurnOnTime(0),
      totalOnTime(0) {
}

PoweredDevice& PoweredDevice::operator=(const PoweredDevice& other) {
    if (this != &other) {
        SmartDevice::operator=(other);
        powerConsumption = other.powerConsumption;
        lastTurnOnTime = 0;
        totalOnTime = 0;
    }
    return *this;
}

void PoweredDevice::turnOn() {
    if (!isOn) {
        isOn = true;
        lastTurnOnTime = time(nullptr);
    }
}

void PoweredDevice::turnOff() {
    if (isOn) {
        isOn = false;
        time_t currentTime = time(nullptr);
        time_t sessionTime = currentTime - lastTurnOnTime;
        totalOnTime += sessionTime;
        
        // Рассчитываем потребленную энергию и добавляем к общей статистике
        double energy = (powerConsumption * sessionTime) / 3600.0; // Используем реальную мощность
        totalEnergyConsumedAll += energy;
    }
}

double PoweredDevice::getTotalOnTime() const {
    if (isOn) {
        return totalOnTime + (time(nullptr) - lastTurnOnTime);
    }
    return totalOnTime;
}

double PoweredDevice::getCurrentSessionTime() const {
    if (isOn) {
        return time(nullptr) - lastTurnOnTime;
    }
    return 0.0;
}

double PoweredDevice::getDeviceEnergyConsumed() const {
    double totalTime = getTotalOnTime();
    return (powerConsumption * totalTime) / 3600.0; // Используем реальную мощность
}

double PoweredDevice::getTotalEnergyConsumedAll() {
    return totalEnergyConsumedAll;
}

void PoweredDevice::resetEnergyConsumption() {
    totalEnergyConsumedAll = 0.0;
}

std::string PoweredDevice::getFormattedOnTime() const {
    double totalSeconds = getTotalOnTime();
    int hours = static_cast<int>(totalSeconds) / 3600;
    int minutes = (static_cast<int>(totalSeconds) % 3600) / 60;
    int seconds = static_cast<int>(totalSeconds) % 60;
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}

double PoweredDevice::getOnTimeInHours() const {
    return getTotalOnTime() / 3600.0;
}

double PoweredDevice::getPowerConsumption() const {
    return powerConsumption;
}

/**
 * @class LightBulb
 * @brief Умная лампочка с регулировкой яркости и цвета
 * 
 * @details
 * Наследует PoweredDevice и добавляет:
 * - Регулировку яркости (0-100%)
 * - Изменение цвета свечения
 * - Отображение информации о состоянии
 */
class LightBulb : public PoweredDevice {
private:
    int brightness;             ///< Текущая яркость (0-100%)
    std::string color;          ///< Цвет свечения
    
public:
    /**
     * @brief Конструктор умной лампочки
     * @param id Уникальный идентификатор
     * @param name Имя устройства
     * @param power Потребляемая мощность (Вт)
     * @param brightness Начальная яркость (0-100%)
     * @param color Начальный цвет свечения
     * @throws std::invalid_argument если brightness не в диапазоне 0-100
     */
    LightBulb(const std::string& id, const std::string& name, 
              double power, int brightness = 100, const std::string& color = "teplyy belyy");
    
    /**
     * @brief Копирующий конструктор
     * @param other Лампочка для копирования
     */
    LightBulb(const LightBulb& other);
    
    /**
     * @brief Оператор присваивания
     * @param other Лампочка для копирования
     * @return Ссылка на текущий объект
     */
    LightBulb& operator=(const LightBulb& other);
    
    /**
     * @brief Получить текущую мощность
     * @override
     * @return мощность устройства
     */
    virtual double getCurrentPower() const override {
        return isOn ? powerConsumption : 0.0;
    }
    
    /**
     * @brief Получить статус лампочки
     * @override
     * @return Строка с полным описанием состояния
     * @details Формат: "Состояние: [вкл/выкл], Яркость: X%, Цвет: Y"
     */
    virtual std::string getStatus() const override;
    
    /**
     * @brief Получить информацию об устройстве
     * @override
     * @return Строка с расширенной информацией
     */
    virtual std::string getDeviceInfo() const override;
    
    /**
     * @brief Установить яркость лампочки
     * @param level Уровень яркости (0-100%)
     * @throws std::invalid_argument если level не в диапазоне 0-100
     */
    void setBrightness(int level);
    
    /**
     * @brief Установить цвет свечения
     * @param newColor Новый цвет
     */
    void setColor(const std::string& newColor);
    
    /**
     * @brief Получить текущую яркость
     * @return Яркость в процентах
     */
    int getBrightness() const;
    
    /**
     * @brief Получить текущий цвет
     * @return Цвет свечения
     */
    std::string getColor() const;
    
    /**
     * @brief Отобразить полную информацию о лампочке
     * @details Выводит в stdout все параметры лампочки
     */
    void displayInfo() const;
};

// Реализация методов LightBulb
LightBulb::LightBulb(const std::string& id, const std::string& name, 
                     double power, int brightness, const std::string& color)
    : PoweredDevice(id, name, power), brightness(brightness), color(color) {
    if (brightness < 0 || brightness > 100) {
        throw std::invalid_argument("Yarkost' dolznha bit 0-100");
    }
}

LightBulb::LightBulb(const LightBulb& other)
    : PoweredDevice(other), brightness(other.brightness), color(other.color) {
}

LightBulb& LightBulb::operator=(const LightBulb& other) {
    if (this != &other) {
        PoweredDevice::operator=(other);
        brightness = other.brightness;
        color = other.color;
    }
    return *this;
}

std::string LightBulb::getStatus() const {
    std::ostringstream oss;
    oss << "Sostoyanie: " << (isOn ? "vklyuchena" : "viklyuchena")
        << ", Yarkost: " << brightness << "%, Tsvet: " << color;
    return oss.str();
}

std::string LightBulb::getDeviceInfo() const {
    std::ostringstream oss;
    oss << "Lampochka: " << deviceName 
        << " (ID: " << deviceId 
        << ", Moshchnost: " << powerConsumption
        << " Vt, Yarkost: " << brightness 
        << "%, Tsvet: " << color << ")";
    return oss.str();
}

void LightBulb::setBrightness(int level) {
    if (level < 0 || level > 100) {
        throw std::invalid_argument("Yarkost' dolznha bit 0-100");
    }
    brightness = level;
}

void LightBulb::setColor(const std::string& newColor) {
    color = newColor;
}

int LightBulb::getBrightness() const {
    return brightness;
}

std::string LightBulb::getColor() const {
    return color;
}

void LightBulb::displayInfo() const {
    std::cout << getDeviceInfo() << "\n";
    std::cout << getStatus() << "\n";
    std::cout << "Obshchee vremya raboty: " << getFormattedOnTime() << "\n";
    std::cout << "Potreblennaya energiya: " << getDeviceEnergyConsumed() << " Vt*ch\n";
}

/**
 * @class Thermostat
 * @brief Умный термостат для измерения температуры
 * 
 * @details
 * Наследует PoweredDevice и добавляет:
 * - Контроль текущей температуры
 * - Режимы работы (отображение/мониторинг)
 */
class Thermostat : public PoweredDevice {
private:
    double currentTemperature;  ///< Текущая температура (°C)
    std::string mode;           ///< Режим работы
    
public:
    /**
     * @brief Конструктор термостата
     * @param id Уникальный идентификатор
     * @param name Имя устройства
     * @param power Потребляемая мощность (Вт)
     * @param initialTemp Начальная температура
     */
    Thermostat(const std::string& id, const std::string& name, 
               double power, double initialTemp = 20.0);
    
    /**
     * @brief Копирующий конструктор
     * @param other Термостат для копирования
     */
    Thermostat(const Thermostat& other);
    
    /**
     * @brief Оператор присваивания
     * @param other Термостат для копирования
     * @return Ссылка на текущий объект
     */
    Thermostat& operator=(const Thermostat& other);
    
    /**
     * @brief Получить текущую мощность
     * @override
     * @return мощность устройства
     */
    virtual double getCurrentPower() const override {
        return isOn ? powerConsumption : 0.0;
    }
    
    /**
     * @brief Включить термостат
     * @override
     * @post Включает режим мониторинга температуры
     */
    virtual void turnOn() override;
    
    /**
     * @brief Выключить термостат
     * @override
     * @post Отключает температурный мониторинг
     */
    virtual void turnOff() override;
    
    /**
     * @brief Получить статус термостата
     * @override
     * @return Строка с полным описанием состояния
     */
    virtual std::string getStatus() const override;
    
    /**
     * @brief Получить информацию об устройстве
     * @override
     * @return Строка с расширенной информацией
     */
    virtual std::string getDeviceInfo() const override;
    
    /**
     * @brief Обновить текущую температуру
     * @param newTemp Новая текущая температура
     */
    void updateTemperature(double newTemp);
    
    /**
     * @brief Установить режим работы
     * @param newMode Новый режим ("display"/"monitoring")
     */
    void setMode(const std::string& newMode);
    
    /**
     * @brief Получить текущую температуру
     * @return Текущая температура (°C)
     */
    double getCurrentTemperature() const;
    
    /**
     * @brief Получить текущий режим работы
     * @return Режим работы
     */
    std::string getMode() const;
    
    /**
     * @brief Отобразить полную информацию о термостате
     */
    void displayInfo() const;
};

// Реализация методов Thermostat
Thermostat::Thermostat(const std::string& id, const std::string& name, 
                       double power, double initialTemp)
    : PoweredDevice(id, name, power), currentTemperature(initialTemp), mode("display") {
}

Thermostat::Thermostat(const Thermostat& other)
    : PoweredDevice(other), currentTemperature(other.currentTemperature), mode(other.mode) {
}

Thermostat& Thermostat::operator=(const Thermostat& other) {
    if (this != &other) {
        PoweredDevice::operator=(other);
        currentTemperature = other.currentTemperature;
        mode = other.mode;
    }
    return *this;
}

void Thermostat::turnOn() {
    if (!isOn) {
        isOn = true;
        lastTurnOnTime = time(nullptr);
        mode = "monitoring";
    }
}

void Thermostat::turnOff() {
    if (isOn) {
        PoweredDevice::turnOff();
        mode = "display";
    }
}

std::string Thermostat::getStatus() const {
    std::ostringstream oss;
    oss << "Sostoyanie: " << (isOn ? "vklyuchen" : "viklyuchen")
        << ", Temperatura: " << std::fixed << std::setprecision(1) << currentTemperature 
        << "°C, Rezhim: " << mode;
    return oss.str();
}

std::string Thermostat::getDeviceInfo() const {
    std::ostringstream oss;
    oss << "Termostat: " << deviceName 
        << " (ID: " << deviceId 
        << ", Moshchnost: " << powerConsumption
        << " Vt, Tekushchaya temp: " << std::fixed << std::setprecision(1) << currentTemperature 
        << "°C)";
    return oss.str();
}

void Thermostat::updateTemperature(double newTemp) {
    currentTemperature = newTemp;
}

void Thermostat::setMode(const std::string& newMode) {
    if (newMode != "display" && newMode != "monitoring") {
        throw std::invalid_argument("Rezhim dolzhen byt' ili monitoring ili display");
    }
    mode = newMode;
}

double Thermostat::getCurrentTemperature() const {
    return currentTemperature;
}

std::string Thermostat::getMode() const {
    return mode;
}

void Thermostat::displayInfo() const {
    std::cout << getDeviceInfo() << "\n";
    std::cout << getStatus() << "\n";
    std::cout << "Obshchee vremya raboty: " << getFormattedOnTime() << "\n";
    std::cout << "Potreblennaya energiya: " << getDeviceEnergyConsumed() << " Vt*ch\n";
}

/**
 * @class SmartOutlet
 * @brief Умная розетка с функцией датчика мощности
 * 
 * @details
 * Использует множественное наследование от PoweredDevice и ISensor.
 * Предоставляет:
 * - Управление подачей питания
 * - Мониторинг мощности
 */
class SmartOutlet : public PoweredDevice, public ISensor {
private:
    bool outletOn;              ///< Состояние розетки (включена/выключена)
    
public:
    /**
     * @brief Конструктор умной розетки
     * @param id Уникальный идентификатор
     * @param name Имя устройства
     * @param power Потребляемая мощность самой розетки (Вт)
     */
    SmartOutlet(const std::string& id, const std::string& name, 
                double power);
    
    /**
     * @brief Копирующий конструктор
     * @param other Розетка для копирования
     */
    SmartOutlet(const SmartOutlet& other);
    
    /**
     * @brief Оператор присваивания
     * @param other Розетка для копирования
     * @return Ссылка на текущий объект
     */
    SmartOutlet& operator=(const SmartOutlet& other);
    
    /**
     * @brief Получить текущую мощность
     * @override
     * @return Текущая мощность в ваттах
     */
    virtual double getCurrentPower() const override {
        return (isOn && outletOn) ? powerConsumption : 0.0;
    }
    
    /**
     * @brief Получить тип датчика
     * @override
     * @return "Datchik Protechki"
     */
    virtual std::string getSensorType() const override {
        return "Datchik Protechki";
    }
    
    /**
     * @brief Включить умную розетку
     * @override
     * @post Включает подачу питания через розетку
     */
    virtual void turnOn() override;
    
    /**
     * @brief Выключить умную розетку
     * @override
     * @post Отключает подачу питания через розетку
     */
    virtual void turnOff() override;
    
    /**
     * @brief Получить статус розетки
     * @override
     * @return Строка с полным описанием состояния
     */
    virtual std::string getStatus() const override;
    
    /**
     * @brief Получить информацию об устройстве
     * @override
     * @return Строка с расширенной информацией
     */
    virtual std::string getDeviceInfo() const override;
    
    /**
     * @brief Переключить состояние розетки
     * @post Изменяет состояние outletOn на противоположное
     */
    void toggleOutlet();
    
    /**
     * @brief Проверить, включена ли розетка
     * @return true если розетка включена
     */
    bool isOutletOn() const;
    
    /**
     * @brief Отобразить полную информацию о розетке
     */
    void displayInfo() const;
};

// Реализация методов SmartOutlet
SmartOutlet::SmartOutlet(const std::string& id, const std::string& name, double power)
    : PoweredDevice(id, name, power), outletOn(false) {
}

SmartOutlet::SmartOutlet(const SmartOutlet& other)
    : PoweredDevice(other), ISensor(), outletOn(other.outletOn) {
}

SmartOutlet& SmartOutlet::operator=(const SmartOutlet& other) {
    if (this != &other) {
        PoweredDevice::operator=(other);
        outletOn = other.outletOn;
    }
    return *this;
}

void SmartOutlet::turnOn() {
    if (!isOn) {
        PoweredDevice::turnOn();
    }
}

void SmartOutlet::turnOff() {
    if (isOn) {
        PoweredDevice::turnOff();
        outletOn = false;
    }
}

std::string SmartOutlet::getStatus() const {
    std::ostringstream oss;
    oss << "Sostoyanie: " << (isOn ? "vklyuchena" : "viklyuchena")
        << ", Rozetka: " << (outletOn ? "vklyuchena" : "viklyuchena")
        << ", Moshchnost: " << std::fixed << std::setprecision(1) << getCurrentPower() << " Vt";
    return oss.str();
}

std::string SmartOutlet::getDeviceInfo() const {
    std::ostringstream oss;
    oss << "Rozetka: " << deviceName 
        << " (ID: " << deviceId 
        << ", Moshchnost: " << powerConsumption 
        << " Vt)";
    return oss.str();
}

void SmartOutlet::toggleOutlet() {
    if (isOn) {
        outletOn = !outletOn;
    }
}

bool SmartOutlet::isOutletOn() const {
    return outletOn && isOn;
}

void SmartOutlet::displayInfo() const {
    std::cout << getDeviceInfo() << "\n";
    std::cout << getStatus() << "\n";
    std::cout << "Tip datchika: " << getSensorType() << "\n";
    std::cout << "Obshchee vremya raboty: " << getFormattedOnTime() << "\n";
    std::cout << "Potreblennaya energiya: " << getDeviceEnergyConsumed() << " Vt*ch\n";
}

#endif