#include "smart_devices.hpp"
#include <cstring>
#include <limits>

// Реализация статических членов SmartDevice
int SmartDevice::totalDevicesCreated = 0;

int SmartDevice::getTotalDevicesCreated() {
    return totalDevicesCreated;
}

// Реализация PoweredDevice
double PoweredDevice::totalEnergyConsumedAll = 0.0;

PoweredDevice::PoweredDevice(const std::string& id, const std::string& name, double power)
    : SmartDevice(id, name), powerConsumption(power), 
      lastTurnOnTime(0), totalOnTime(0) {}

PoweredDevice::PoweredDevice(const PoweredDevice& other)
    : SmartDevice(other), 
      powerConsumption(other.powerConsumption),
      lastTurnOnTime(other.lastTurnOnTime),
      totalOnTime(other.totalOnTime) {}

PoweredDevice& PoweredDevice::operator=(const PoweredDevice& other) {
    if (this != &other) {
        SmartDevice::operator=(other);
        powerConsumption = other.powerConsumption;
        lastTurnOnTime = other.lastTurnOnTime;
        totalOnTime = other.totalOnTime;
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
        time_t onDuration = currentTime - lastTurnOnTime;
        totalOnTime += onDuration;
        
        double hours = onDuration / 3600.0;
        double energy = powerConsumption * hours;
        totalEnergyConsumedAll += energy;
    }
}

double PoweredDevice::getPowerUsage() const {
    return isOn ? powerConsumption : 0;
}

double PoweredDevice::getTotalOnTime() const { 
    return totalOnTime; 
}

double PoweredDevice::getCurrentSessionTime() const {
    if (isOn) {
        return difftime(time(nullptr), lastTurnOnTime);
    }
    return 0;
}

double PoweredDevice::getDeviceEnergyConsumed() const {
    double totalHours = totalOnTime / 3600.0;
    if (isOn) {
        totalHours += getCurrentSessionTime() / 3600.0;
    }
    return powerConsumption * totalHours;
}

double PoweredDevice::getTotalEnergyConsumedAll() {
    return totalEnergyConsumedAll;
}

void PoweredDevice::resetEnergyConsumption() {
    totalEnergyConsumedAll = 0;
}

std::string PoweredDevice::getFormattedOnTime() const {
    time_t totalSeconds = totalOnTime;
    if (isOn) {
        totalSeconds += getCurrentSessionTime();
    }
    
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    std::stringstream ss;
    if (hours > 0) ss << hours << "ch ";
    if (minutes > 0 || hours > 0) ss << minutes << "min ";
    ss << seconds << "s";
    return ss.str();
}

double PoweredDevice::getOnTimeInHours() const {
    return (totalOnTime + (isOn ? getCurrentSessionTime() : 0)) / 3600.0;
}

double PoweredDevice::getPowerConsumption() const { 
    return powerConsumption; 
}

// Реализация LightBulb
LightBulb::LightBulb(const std::string& id, const std::string& name, 
          double power, int brightness, const std::string& color)
    : PoweredDevice(id, name, power), brightness(brightness), color(color) {
    if (brightness < 0 || brightness > 100) {
        throw std::invalid_argument("Yarkost' dolzhna byt' ot 0 do 100");
    }
}

LightBulb::LightBulb(const LightBulb& other)
    : PoweredDevice(other), brightness(other.brightness), color(other.color) {}

LightBulb& LightBulb::operator=(const LightBulb& other) {
    if (this != &other) {
        PoweredDevice::operator=(other);
        brightness = other.brightness;
        color = other.color;
    }
    return *this;
}

std::string LightBulb::getStatus() const {
    std::stringstream ss;
    ss << "Lampochka " << deviceName << " " << (isOn ? "VKLYUCHENA" : "VYKLYUCHENA");
    ss << ", Yarkost': " << brightness << "%, Tsvet: " << color
       << ", Potreblenie: " << powerConsumption << " Vt";
    return ss.str();
}

std::string LightBulb::getDeviceInfo() const {
    return PoweredDevice::getDeviceInfo() + " [Lampochka]";
}

void LightBulb::setBrightness(int level) {
    if (level >= 0 && level <= 100) {
        brightness = level;
    } else {
        throw std::invalid_argument("Yarkost' dolzhna byt' ot 0 do 100%");
    }
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
    std::cout << "=== Informatsiya o lampochke ===\n";
    std::cout << "Nazvanie: " << deviceName << "\n";
    std::cout << "Status: " << (isOn ? "VKLYUCHENA" : "VYKLYUCHENA") << "\n";
    std::cout << "Yarkost': " << brightness << "%\n";
    std::cout << "Tsvet: " << color << "\n";
    std::cout << "Potreblenie: " << powerConsumption << " Vt\n";
    std::cout << "Obshchee vremya raboty: " << getFormattedOnTime() << "\n";
    std::cout << "Potracheno energii: " << std::fixed << std::setprecision(4) 
              << getDeviceEnergyConsumed() << " Vt\n";
}

// Реализация Thermostat
Thermostat::Thermostat(const std::string& id, const std::string& name, 
           double power, double initialTemp)
    : PoweredDevice(id, name, power), currentTemperature(initialTemp),
      targetTemperature(initialTemp), mode("off") {}

Thermostat::Thermostat(const Thermostat& other)
    : PoweredDevice(other), currentTemperature(other.currentTemperature),
      targetTemperature(other.targetTemperature), mode(other.mode) {}

Thermostat& Thermostat::operator=(const Thermostat& other) {
    if (this != &other) {
        PoweredDevice::operator=(other);
        currentTemperature = other.currentTemperature;
        targetTemperature = other.targetTemperature;
        mode = other.mode;
    }
    return *this;
}

void Thermostat::turnOn() {
    PoweredDevice::turnOn();
    if (mode == "off") {
        mode = "heating";
    }
}

void Thermostat::turnOff() {
    PoweredDevice::turnOff();
    mode = "off";
}

std::string Thermostat::getStatus() const {
    std::stringstream ss;
    ss << "Termostat " << deviceName << " " << (isOn ? "VKLYUCHEN" : "VYKLYUCHEN");
    ss << ", Tekushchaya: " << std::fixed << std::setprecision(1) << currentTemperature
       << "C, Tselevaya: " << targetTemperature << "C, Rezhim: " << mode
       << ", Potreblenie: " << powerConsumption << " Vt";
    return ss.str();
}

std::string Thermostat::getDeviceInfo() const {
    return PoweredDevice::getDeviceInfo() + " [Termostat]";
}

double Thermostat::getPowerUsage() const {
    if (!isOn || mode == "off") {
        return 0;
    }
    double tempDiff = std::abs(targetTemperature - currentTemperature);
    return powerConsumption * (0.5 + tempDiff / 10.0);
}

void Thermostat::setTargetTemperature(double temp) {
    targetTemperature = temp;
    if (!isOn && temp != currentTemperature) {
        turnOn();
    }
}

void Thermostat::updateTemperature(double newTemp) {
    currentTemperature = newTemp;
}

void Thermostat::setMode(const std::string& newMode) {
    if (newMode == "heating" || newMode == "cooling" || newMode == "off") {
        mode = newMode;
        if (newMode != "off" && !isOn) {
            turnOn();
        }
    } else {
        throw std::invalid_argument("Rezhim dolzhen byt' 'heating', 'cooling' ili 'off'");
    }
}

double Thermostat::getCurrentTemperature() const { 
    return currentTemperature; 
}

double Thermostat::getTargetTemperature() const { 
    return targetTemperature; 
}

std::string Thermostat::getMode() const { 
    return mode; 
}

void Thermostat::displayInfo() const {
    std::cout << "=== Informatsiya o termostate ===\n";
    std::cout << "Nazvanie: " << deviceName << "\n";
    std::cout << "Status: " << (isOn ? "VKLYUCHEN" : "VYKLYUCHEN") << "\n";
    std::cout << "Tekushchaya temperatura: " << currentTemperature << "C\n";
    std::cout << "Tselevaya temperatura: " << targetTemperature << "C\n";
    std::cout << "Rezhim: " << mode << "\n";
    std::cout << "Potreblenie: " << powerConsumption << " Vt\n";
    std::cout << "Tekushchee potreblenie: " << getPowerUsage() << " Vt\n";
    std::cout << "Obshchee vremya raboty: " << getFormattedOnTime() << "\n";
    std::cout << "Potracheno energii: " << std::fixed << std::setprecision(4) 
              << getDeviceEnergyConsumed() << " Vt\n";
}

// Реализация SmartOutlet
SmartOutlet::SmartOutlet(const std::string& id, const std::string& name, 
            double power, double maxCurrent)
    : PoweredDevice(id, name, power), outletOn(false), 
      voltage(220.0), maxCurrent(maxCurrent) {}

SmartOutlet::SmartOutlet(const SmartOutlet& other)
    : PoweredDevice(other), ISensor(),
      outletOn(other.outletOn), voltage(other.voltage),
      maxCurrent(other.maxCurrent) {}

SmartOutlet& SmartOutlet::operator=(const SmartOutlet& other) {
    if (this != &other) {
        PoweredDevice::operator=(other);
        outletOn = other.outletOn;
        voltage = other.voltage;
        maxCurrent = other.maxCurrent;
    }
    return *this;
}

double SmartOutlet::getCurrentVoltage() const {
    return voltage + ((std::rand() % 200 - 100) / 100.0);
}

std::string SmartOutlet::getSensorType() const {
    return "Datchik napryazheniya";
}

void SmartOutlet::turnOn() {
    PoweredDevice::turnOn();
    outletOn = true;
}

void SmartOutlet::turnOff() {
    PoweredDevice::turnOff();
    outletOn = false;
}

std::string SmartOutlet::getStatus() const {
    std::stringstream ss;
    ss << "Umnaya rozetka " << deviceName << " " << (isOn ? "VKLYUCHENA" : "VYKLYUCHENA");
    ss << ", Rozetka: " << (outletOn ? "aktivna" : "neaktivna")
       << ", Maks. tok: " << maxCurrent << " A"
       << ", Potreblenie: " << powerConsumption << " Vt";
    return ss.str();
}

std::string SmartOutlet::getDeviceInfo() const {
    return PoweredDevice::getDeviceInfo() + " [Umnaya rozetka s datchikom]";
}

double SmartOutlet::getPowerUsage() const {
    if (!isOn || !outletOn) {
        return 0;
    }
    return powerConsumption;
}

void SmartOutlet::toggleOutlet() {
    outletOn = !outletOn;
}

bool SmartOutlet::isOutletOn() const { 
    return outletOn; 
}

double SmartOutlet::getMaxCurrent() const { 
    return maxCurrent; 
}

double SmartOutlet::getVoltage() const { 
    return voltage; 
}

void SmartOutlet::displayInfo() const {
    std::cout << "=== Informatsiya ob umnoy rozetke ===\n";
    std::cout << "Nazvanie: " << deviceName << "\n";
    std::cout << "Status ustroystva: " << (isOn ? "VKLYUCHEN" : "VYKLYUCHEN") << "\n";
    std::cout << "Status rozetki: " << (outletOn ? "aktivna" : "neaktivna") << "\n";
    std::cout << "Tip datchika: " << getSensorType() << "\n";
    std::cout << "Tekushchee napryazhenie: " << std::fixed << std::setprecision(1) 
              << getCurrentVoltage() << " V\n";
    std::cout << "Maksimal'nyy tok: " << maxCurrent << " A\n";
    std::cout << "Potreblenie: " << powerConsumption << " Vt\n";
    std::cout << "Obshchee vremya raboty: " << getFormattedOnTime() << "\n";
    std::cout << "Potracheno energii: " << std::fixed << std::setprecision(4) 
              << getDeviceEnergyConsumed() << " Vt\n";
}

// ===== ТЕСТОВЫЕ ФУНКЦИИ =====

// Тест 1: Создание устройств и базовые операции
void test1_basicOperations() {
    std::cout << "\n=== TEST 1: Osnovnye operatsii ===\n";
    
    // Создаем по одному устройству каждого типа
    LightBulb lamp("LB1", "Lampochka v gostinoy", 60, 75, "teplyy belyy");
    Thermostat thermo("TH1", "Termostat v spalne", 1000, 22.5);
    SmartOutlet outlet("SO1", "Rozetka v koridore", 5, 16);
    
    std::cout << "Sozdano 3 ustroystva:\n";
    std::cout << "1. " << lamp.getDeviceInfo() << "\n";
    std::cout << "2. " << thermo.getDeviceInfo() << "\n";
    std::cout << "3. " << outlet.getDeviceInfo() << "\n";
    
    std::cout << "\nVsego ustroystv v sisteme: " 
              << SmartDevice::getTotalDevicesCreated() << "\n";
    
    std::cout << "\nStatus ustroystv:\n";
    std::cout << lamp.getStatus() << "\n";
    std::cout << thermo.getStatus() << "\n";
    std::cout << outlet.getStatus() << "\n";
}

// Тест 2: Полиморфизм
void test2_polymorphism() {
    std::cout << "\n=== TEST 2: Polimorfizm ===\n";
    
    // Создаем устройства
    LightBulb lamp("LB1", "Lampochka", 60, 75, "belyy");
    Thermostat thermo("TH1", "Termostat", 1000, 22.0);
    SmartOutlet outlet("SO1", "Rozetka", 5, 16);
    
    // Создаем массив указателей на базовый класс
    SmartDevice* devices[3];
    devices[0] = &lamp;
    devices[1] = &thermo;
    devices[2] = &outlet;
    
    std::cout << "Vklyuchenie vsekh ustroystv cherez bazovyy ukazatel':\n";
    for (int i = 0; i < 3; i++) {
        devices[i]->turnOn();
        std::cout << devices[i]->getStatus() << std::endl;
    }
    
    std::cout << "\nVyklyuchenie vsekh ustroystv:\n";
    for (int i = 0; i < 3; i++) {
        devices[i]->turnOff();
        std::cout << devices[i]->getStatus() << std::endl;
    }
}

// Тест 3: Статические члены
void test3_staticMembers() {
    std::cout << "\n=== TEST 3: Staticheskie chleny ===\n";
    
    // Сброс статической статистики
    PoweredDevice::resetEnergyConsumption();
    
    LightBulb lamp("LB_STAT", "Test lampa", 60, 50, "belyy");
    Thermostat thermo("TH_STAT", "Test termostat", 800, 21.0);
    SmartOutlet outlet("SO_STAT", "Test rozetka", 5, 16);
    
    // Включаем и выключаем устройства
    lamp.turnOn();
    thermo.turnOn();
    outlet.turnOn();
    
    // Имитируем работу устройств
    std::cout << "Ustroystva vklyucheny. Priblizitel'noe vremya raboty: 1 sekunda...\n";
    
    // Просто ждем немного (без потоков)
    for (int i = 0; i < 100000000; i++) {
        // Пустой цикл для имитации задержки
    }
    
    lamp.turnOff();
    thermo.turnOff();
    outlet.turnOff();
    
    std::cout << "\nStatistika posle raboty:\n";
    std::cout << "  Vsego sozdano ustroystv: " << SmartDevice::getTotalDevicesCreated() << "\n";
    std::cout << "  Obshchaya potreblennaya energiya: " 
              << PoweredDevice::getTotalEnergyConsumedAll() << " Vt\n";
}

// Тест 4: Копирование и присваивание
void test4_copyAndAssignment() {
    std::cout << "\n=== TEST 4: Kopirovanie i prisvaivanie ===\n";
    
    // Создаем оригинальные объекты
    LightBulb lamp1("LB1", "Lampochka 1", 60, 75, "belyy");
    Thermostat thermo1("TH1", "Termostat 1", 1000, 22.5);
    SmartOutlet outlet1("SO1", "Rozetka 1", 5, 16);
    
    // Тестируем конструктор копирования
    LightBulb lamp2 = lamp1;  // Конструктор копирования
    Thermostat thermo2(thermo1);  // Конструктор копирования
    
    // Тестируем оператор присваивания
    SmartOutlet outlet2("SO2", "Rozetka 2", 10, 20);
    outlet2 = outlet1;  // Оператор присваивания
    
    // Проверяем, что копии независимы
    lamp1.turnOn();
    lamp2.turnOff();
    
    std::cout << "Original lampochka: " << (lamp1.getIsOn() ? "vklyuchena" : "viklyuchena") << "\n";
    std::cout << "Kopiya lampochki: " << (lamp2.getIsOn() ? "vklyuchena" : "viklyuchena") << "\n";
    std::cout << "  (dolzhny byt' raznye - kopiya nezavisima)\n";
    
    // Проверяем ID
    std::cout << "\nProverka ID:\n";
    std::cout << "  Original lampochka ID: " << lamp1.getId() << "\n";
    std::cout << "  Kopiya lampochki ID: " << lamp2.getId() << "\n";
    std::cout << "  (dolzhny otlichat'sya sufiksom '_copy')\n";
}

// Тест 5: Множественное наследование
void test5_multipleInheritance() {
    std::cout << "\n=== TEST 5: Mnozhestvennoe nasledovanie ===\n";
    
    SmartOutlet outlet("SO_MULT", "Rozetka s datchikom", 5, 16);
    
    // Работаем как с PoweredDevice
    outlet.turnOn();
    outlet.toggleOutlet();
    std::cout << "Kak PoweredDevice: " << outlet.getStatus() << "\n";
    
    // Работаем как с ISensor
    ISensor* sensor = &outlet;  // Приведение к базовому классу
    std::cout << "Kak ISensor:\n";
    std::cout << "  Tip datchika: " << sensor->getSensorType() << "\n";
    std::cout << "  Napryazhenie: " << sensor->getCurrentVoltage() << " V\n";
    
    // Динамическое приведение типов
    SmartDevice* device = &outlet;
    ISensor* sensor2 = dynamic_cast<ISensor*>(device);
    
    if (sensor2) {
        std::cout << "Uspeshnoe dynamicheskoe privedenie k ISensor!\n";
    }
}

// Тест 6: Обработка исключений
void test6_exceptionHandling() {
    std::cout << "\n=== TEST 6: Obrabotka isklyucheniy ===\n";
    
    LightBulb lamp("LB_EX", "Test lampa", 60, 50, "belyy");
    Thermostat thermo("TH_EX", "Test termostat", 1000, 22.0);
    
    std::cout << "Test 1: Nepravil'naya yarkost' lampochki\n";
    try {
        lamp.setBrightness(150);  // Недопустимое значение
        std::cout << "  ERROR: Isklyuchenie ne bylo vybrosheno!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "  OK: Poymalo isklyuchenie: " << e.what() << "\n";
    }
    
    std::cout << "\nTest 2: Nepravil'nyy rezhim termostata\n";
    try {
        thermo.setMode("neizvestnyy");  // Недопустимый режим
        std::cout << "  ERROR: Isklyuchenie ne bylo vybrosheno!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "  OK: Poymalo isklyuchenie: " << e.what() << "\n";
    }
    
    std::cout << "\nTest 3: Pravil'nye znacheniya (isklyucheniy net)\n";
    try {
        lamp.setBrightness(80);  // Допустимое значение
        thermo.setMode("cooling");  // Допустимый режим
        std::cout << "  OK: Isklyucheniy net, vse rabotaet\n";
    } catch (...) {
        std::cout << "  ERROR: Nepredvidennoe isklyuchenie!\n";
    }
}

// Тест 7: Динамическая память
void test7_dynamicMemory() {
    std::cout << "\n=== TEST 7: Dinamicheskaya pamyat' ===\n";
    
    // Создаем устройства в динамической памяти
    SmartDevice* lamp = new LightBulb("LB_DYN", "Dinamicheskaya lampa", 60, 75, "zelenyy");
    SmartDevice* thermo = new Thermostat("TH_DYN", "Dinamicheskiy termostat", 800, 23.0);
    SmartDevice* outlet = new SmartOutlet("SO_DYN", "Dinamicheskaya rozetka", 5, 16);
    
    std::cout << "Sozdano 3 ustroystva v dinamicheskoy pamyati\n";
    
    // Создаем копии
    SmartDevice* lampCopy = new LightBulb(*dynamic_cast<LightBulb*>(lamp));
    SmartDevice* thermoCopy = new Thermostat(*dynamic_cast<Thermostat*>(thermo));
    SmartDevice* outletCopy = new SmartOutlet(*dynamic_cast<SmartOutlet*>(outlet));
    
    std::cout << "Vypolneno glubokoe kopirovanie ustroystv\n";
    
    // Проверяем независимость
    if (LightBulb* orig = dynamic_cast<LightBulb*>(lamp)) {
        if (LightBulb* cpy = dynamic_cast<LightBulb*>(lampCopy)) {
            orig->setBrightness(100);
            cpy->setBrightness(50);
            
            std::cout << "\nProverka nezavisimosti kopiy:\n";
            std::cout << "Original yarkost': " << orig->getBrightness() << "%\n";
            std::cout << "Kopiya yarkost': " << cpy->getBrightness() << "%\n";
            std::cout << "  (raznye znacheniya - kopii nezavisimy)\n";
        }
    }
    
    // Очистка памяти
    delete lamp;
    delete thermo;
    delete outlet;
    delete lampCopy;
    delete thermoCopy;
    delete outletCopy;
    
    std::cout << "Pamyat' uspeshno ochishchena\n";
}

// Тест 8: Полная демонстрация
void test8_fullDemo() {
    std::cout << "\n=== TEST 8: POLNAYA DEMONSTRATSIYA ===\n";
    
    // Создаем устройства
    LightBulb lamp("LB_FULL", "Luchshaya lampochka", 60, 85, "teplyy belyy");
    Thermostat thermo("TH_FULL", "Umnyy termostat", 1000, 23.5);
    SmartOutlet outlet("SO_FULL", "Intelligentnaya rozetka", 5, 16);
    
    std::cout << "\n--- Chast 1: Sozdanie i osnovnaya informatsiya ---\n";
    std::cout << lamp.getDeviceInfo() << "\n";
    std::cout << thermo.getDeviceInfo() << "\n";
    std::cout << outlet.getDeviceInfo() << "\n";
    
    std::cout << "\n--- Chast 2: Vklyuchenie i rabota ---\n";
    lamp.turnOn();
    thermo.turnOn();
    thermo.setTargetTemperature(25.0);
    thermo.setMode("heating");
    outlet.turnOn();
    outlet.toggleOutlet();
    
    std::cout << "Tekushchiy status:\n";
    std::cout << lamp.getStatus() << "\n";
    std::cout << thermo.getStatus() << "\n";
    std::cout << outlet.getStatus() << "\n";
    
    std::cout << "\n--- Chast 3: Izmenenie parametrov ---\n";
    lamp.setBrightness(95);
    lamp.setColor("goluboy");
    thermo.updateTemperature(24.0);
    
    std::cout << "Posle izmeneniy:\n";
    std::cout << "Lampochka: yarkost' " << lamp.getBrightness() << "%, tsvet " << lamp.getColor() << "\n";
    std::cout << "Termostat: tekushchaya temp. " << thermo.getCurrentTemperature() << "C\n";
    
    std::cout << "\n--- Chast 4: Podrobnaya informatsiya ---\n";
    lamp.displayInfo();
    std::cout << "\n";
    thermo.displayInfo();
    std::cout << "\n";
    outlet.displayInfo();
    
    std::cout << "\n--- Chast 5: Vyklyuchenie i statistika ---\n";
    lamp.turnOff();
    thermo.turnOff();
    outlet.turnOff();
    
    std::cout << "Vsego sozdano ustroystv: " << SmartDevice::getTotalDevicesCreated() << "\n";
    std::cout << "Obshchee potreblenie energii: " << PoweredDevice::getTotalEnergyConsumedAll() << " Vt\n";
}

// Функция для отображения меню
void showMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "    TESTIROVANIE UMNOGO DOMA\n";
    std::cout << "=========================================\n";
    std::cout << "1. Osnovnye operatsii (sozdanie, status)\n";
    std::cout << "2. Polimorfizm (virtual'nye funktsii)\n";
    std::cout << "3. Staticheskie chleny (statistika)\n";
    std::cout << "4. Kopirovanie i prisvaivanie\n";
    std::cout << "5. Mnozhestvennoe nasledovanie\n";
    std::cout << "6. Obrabotka isklyucheniy\n";
    std::cout << "7. Rabota s dinamicheskoy pamyat'yu\n";
    std::cout << "8. POLNAYA DEMONSTRATSIYA (vse testy)\n";
    std::cout << "9. Vykhod\n";
    std::cout << "=========================================\n";
    std::cout << "Vash vybor (1-9): ";
}

// Основная тестовая функция с меню
int main() {
    std::srand(std::time(nullptr));
    
    int choice = 0;
    
    do {
        showMenu();
        std::cin >> choice;
        
        // Очищаем буфер ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                test1_basicOperations();
                break;
                
            case 2:
                test2_polymorphism();
                break;
                
            case 3:
                test3_staticMembers();
                break;
                
            case 4:
                test4_copyAndAssignment();
                break;
                
            case 5:
                test5_multipleInheritance();
                break;
                
            case 6:
                test6_exceptionHandling();
                break;
                
            case 7:
                test7_dynamicMemory();
                break;
                
            case 8:
                test8_fullDemo();
                break;
                
            case 9:
                std::cout << "\nVykhod iz programmy...\n";
                break;
                
            default:
                std::cout << "\nNepravil'nyy vybor! Poprobuyte snova.\n";
                break;
        }
        
        if (choice != 9) {
            std::cout << "\nNazhmite Enter dlya prodolzheniya...";
            std::cin.get();
        }
        
    } while (choice != 9);
    
    // Финальная статистика
    std::cout << "\n=========================================\n";
    std::cout << "FINAL'NAYa STATISTIKA:\n";
    std::cout << "  Vsego sozdano ustroystv: " 
              << SmartDevice::getTotalDevicesCreated() << "\n";
    std::cout << "  Obshchaya potreblennaya energiya: " 
              << PoweredDevice::getTotalEnergyConsumedAll() << " Vt\n";
    std::cout << "=========================================\n";
    
    return 0;
}