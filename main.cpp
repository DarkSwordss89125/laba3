#include <iostream>
#include <windows.h>
#include "smart_devices.hpp"

void polymorphism() {
    std::cout << "\nPolimorfnyy vyzov\n";
    
    LightBulb lamp("LB2", "Lampochka", 20000);
    Thermostat thermo("TH2", "Termostat", 20000);
    SmartOutlet outlet("SO2", "Rozetka", 20000);
    
    SmartDevice* devices[] = {&lamp, &thermo, &outlet};
    
    for (int i = 0; i < 3; i++) {
        SmartDevice* device = devices[i];
        device->turnOn();
        std::cout << device->getStatus() << std::endl;
    }
    
    for (int i = 0; i < 3; i++) {
        SmartDevice* device = devices[i];
        device->turnOff();
        std::cout << device->getStatus() << std::endl;
    }
}

void staticMembers() {
    std::cout << "\nRabota staticheskikh chlenov\n";
    
    int initialDeviceCount = SmartDevice::getTotalDevicesCreated();
    double initialEnergy = PoweredDevice::getTotalEnergyConsumedAll();
    
    {
        LightBulb lamp("LB3", "Test lampa", 60);
        Thermostat thermo("TH3", "Test termostat", 800);
        SmartOutlet outlet("SO3", "Test rozetka", 5);
        
        lamp.turnOn();
        thermo.turnOn();
        outlet.turnOn();
        
        Sleep(500);
        
        lamp.turnOff();
        thermo.turnOff();
        outlet.turnOff();
        
        std::cout << "Sozdano ustroystv v teste: " << (SmartDevice::getTotalDevicesCreated() - initialDeviceCount) << "\n";
        std::cout << "Potrebleno energii v teste: " << (PoweredDevice::getTotalEnergyConsumedAll() - initialEnergy) << " Vt*ch\n";
    }
}

void copyAndAssignment() {
    std::cout << "\nKopirovanie i prisvaivanie\n";
    
    LightBulb lamp1("LB4", "Lampochka 1", 60, 75, "belyy");
    lamp1.turnOn();
    
    LightBulb lamp2 = lamp1;
    LightBulb lamp3("LB4_temp", "Temp", 40);
    lamp3 = lamp1;
    
    std::cout << "Original ID: " << lamp1.getId() << "\n";
    std::cout << "Kopiya ID: " << lamp2.getId() << "\n";
    std::cout << "Prisvoennaya ID: " << lamp3.getId() << "\n";
}

void exceptions() {
    std::cout << "\nObrabotka isklyucheniy\n";
    
    std::cout << "1. Sozdayu lampochku s yarkost'yu 150% (dopustimo 0-100%):\n";
    try {
        LightBulb lamp("LB5", "Test lampa", 60, 150, "belyy");
        std::cout << "  Lampochka sozdana, yarkost: " << lamp.getBrightness() << "%\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "  Vozniklo isklyuchenie: " << e.what() << "\n";
    }
    
    std::cout << "\n2. Sozdayu lampochku s yarkost'yu 50%, zatem pytayus' ustanovit' 150%:\n";
    LightBulb lamp("LB5", "Test lampa", 60, 50);
    std::cout << "  Nachal'naya yarkost': " << lamp.getBrightness() << "%\n";
    
    try {
        std::cout << "  Pytaemsya ustanovit' yarkost' 150%...\n";
        lamp.setBrightness(150);
        std::cout << "  Yarkost' ustanovlena: " << lamp.getBrightness() << "%\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "  Vozniklo isklyuchenie: " << e.what() << "\n";
    }
}

void multipleInheritance() {
    std::cout << "\nMnozhestvennoe nasledovanie\n";
    
    SmartOutlet outlet("SO6", "Rozetka s datchikom protechki", 5);
    outlet.turnOn();
    
    ISensor* sensor = &outlet;
    std::cout << "Tip datchika: " << sensor->getSensorType() << "\n";
    std::cout << "Moshchnost: " << sensor->getCurrentPower() << " Vt\n";
}

SmartDevice* devices[3] = {nullptr, nullptr, nullptr};
int deviceCount = 3;
bool devicesCreated = false;

void createDevices() {
    std::cout << "Sozdanie 3 ustroystv...\n";
    
    if (devicesCreated) {
        for (int i = 0; i < deviceCount; i++) {
            if (devices[i]) {
                delete devices[i];
                devices[i] = nullptr;
            }
        }
    }
    
    devices[0] = new LightBulb("LB1", "Lampochka", 60, 75, "teplyy belyy");
    devices[1] = new Thermostat("TH1", "Termostat", 1000, 22.5);
    devices[2] = new SmartOutlet("SO1", "Rozetka s datchikom protechki", 5);
    
    devicesCreated = true;
    
    std::cout << "Sozdano 3 ustroystva:\n";
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i]) {
            std::cout << i+1 << ". " << devices[i]->getDeviceInfo() << "\n";
            
            if (Thermostat* thermo = dynamic_cast<Thermostat*>(devices[i])) {
                std::cout << "   Rezhim: " << thermo->getMode() << "\n";
            }
        }
    }
}

void turnOnAll() {
    if (!devicesCreated) {
        std::cout << "Net ustroystv! Sozdayte ustroystva snachala (vyberite punkt 2 v menu).\n";
        return;
    }
    
    std::cout << "\n=== Vklyuchenie vsekh ustroystv ===\n";
    int count = 0;
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i]) {
            devices[i]->turnOn();
            SmartOutlet* outlet = dynamic_cast<SmartOutlet*>(devices[i]);
            if (outlet) {
                outlet->toggleOutlet();
            }
            std::cout << devices[i]->getName() << ": " << devices[i]->getStatus() << std::endl;
            count++;
        }
    }
    std::cout << "Vsego vklyucheno: " << count << " ustroystv\n";
}

void turnOffAll() {
    if (!devicesCreated) {
        std::cout << "Net ustroystv! Sozdayte ustroystva snachala (vyberite punkt 2 v menu).\n";
        return;
    }
    
    std::cout << "\n=== Viklyuchenie vsekh ustroystv ===\n";
    int count = 0;
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i]) {
            devices[i]->turnOff();
            std::cout << devices[i]->getName() << ": " << devices[i]->getStatus() << std::endl;
            count++;
        }
    }
    std::cout << "Vsego viklyucheno: " << count << " ustroystv\n";
}

void showStatistics() {
    if (!devicesCreated) {
        std::cout << "Net ustroystv! Sozdayte ustroystva snachala (vyberite punkt 2 v menu).\n";
        return;
    }
    
    std::cout << "\n=== Statistika potrebleniya energii ===\n";
    
    int onDevices = 0;
    double totalCurrentPower = 0.0;
    
    std::cout << "\n=== Potreblenie vklyuchennykh ustroystv ===\n";
    bool hasOnPoweredDevices = false;
    
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i] && devices[i]->getIsOn()) {
            PoweredDevice* poweredDevice = dynamic_cast<PoweredDevice*>(devices[i]);
            if (poweredDevice) {
                hasOnPoweredDevices = true;
                double energyConsumed = poweredDevice->getDeviceEnergyConsumed();
                double currentPower = poweredDevice->getPowerUsage();
                
                std::cout << devices[i]->getName() 
                          << ": Potrebleno energii = " << std::fixed << std::setprecision(3) << energyConsumed << " Vt*ch";
                std::cout << ", Tekushchaya moshchnost = " << currentPower << " Vt";
                
                LightBulb* lamp = dynamic_cast<LightBulb*>(devices[i]);
                if (lamp) {
                    std::cout << ", Yarkost: " << lamp->getBrightness() << "%";
                } 
                else {
                    Thermostat* thermo = dynamic_cast<Thermostat*>(devices[i]);
                    if (thermo) {
                        std::cout << ", Temp: " << thermo->getCurrentTemperature() << "°C";
                    }
                }
                
                std::cout << std::endl;
                
                totalCurrentPower += currentPower;
            }
        }
    }
    
    if (!hasOnPoweredDevices) {
        std::cout << "Net vklyuchennykh ustroystv s uchetom energii\n";
    }
    
    std::cout << "\n=== Obshchaya statistika ===\n";
    std::cout << "Vsego sozdano ustroystv: " << SmartDevice::getTotalDevicesCreated() << "\n";
    std::cout << "Obshchee potreblenie energii vsemi ustroystvami: " 
              << std::fixed << std::setprecision(3) << PoweredDevice::getTotalEnergyConsumedAll() << " Vt*ch\n";
    std::cout << "Obshchaya tekushchaya moshchnost: " << totalCurrentPower << " Vt" << std::endl;
}

void clearDevices() {
    if (devicesCreated) {
        for (int i = 0; i < deviceCount; i++) {
            if (devices[i]) {
                delete devices[i];
                devices[i] = nullptr;
            }
        }
        devicesCreated = false;
        std::cout << "Vse ustroystva udaleny!\n";
    } else {
        std::cout << "Net ustroystv dlya udaleniya!\n";
    }
}

void showMainMenu() {
    std::cout << "\n  TESTIROVANIE UMNOGO DOMA\n";
    std::cout << "1. Sozdat 3 ustroystva\n";
    std::cout << "2. Vklyuchit VSE ustroystva\n";
    std::cout << "3. Viklyuchit VSE ustroystva\n";
    std::cout << "4. Pokazat statistiku\n";
    std::cout << "5. Polimorfizm\n";
    std::cout << "6. Staticheskie chleny\n";
    std::cout << "7. Kopirovanie\n";
    std::cout << "8. Isklyucheniya\n";
    std::cout << "9. Mnozhestvennoe nasledovanie\n";
    std::cout << "10. Udalit' vse ustroystva\n";
    std::cout << "11. Vykhod\n";
    std::cout << "Vash vybor (1-11): ";
}

int main() {
    int choice = 0;
    
    do {
        showMainMenu();
        std::cin >> choice;
        std::cin.ignore(1000, '\n');
        
        switch (choice) {
            case 1: createDevices(); break;
            case 2: turnOnAll(); break;
            case 3: turnOffAll(); break;
            case 4: showStatistics(); break;
            case 5: polymorphism(); break;
            case 6: staticMembers(); break;
            case 7: copyAndAssignment(); break;
            case 8: exceptions(); break;
            case 9: multipleInheritance(); break;
            case 10: clearDevices(); break;
            case 11: 
                std::cout << "\nVykhod...\n";
                clearDevices(); 
                break;
            default: std::cout << "\nNepravil'nyy vybor!\n";
        }
        
        if (choice != 11) {
            std::cout << "\nNazhmite Enter...";
            std::cin.get();
        }
        
    } while (choice != 11);
    
    return 0;
}