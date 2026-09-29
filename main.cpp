#include "tariff.h"
#include "sensor.h"
#include <iostream>

int main() {
    greenhouse::Tariff dayTariff("Дневной", 1.25);
    greenhouse::Sensor tSensor("Датчик температуры", &dayTariff);
    tSensor.PrintInfo();
    std::cout << "\n";
    tSensor.TakeMeasurement();
    std::cout << "\n";
    tSensor.Recharge();
    return 0;
}