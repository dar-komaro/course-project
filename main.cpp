#include "tariff.hpp"
#include "sensor.hpp"
#include "greenhouse.hpp"
#include <iostream>

int main() {
    greenhouse::Tariff tarrif("Обычный", 1.25);
    {
        std::cout << "Вложенный блок\n";
        greenhouse::Greenhouse gHouse("Теплица 1", &tarrif);

        //Ссылка
        greenhouse::Greenhouse& h = gHouse;
        h.PrintStatus();
        std::cout << "\n\n";

        std::cout << "Нормальный опрос\n";
        h.FullInspection();

        //Нарушение правила: батарея датчика = 0 
        for (int i = 0; i < 19; i++) {
            h.GetSensor(2).TakeMeasurement();
        }
        std::cout << "\nПопытка опроса с разряженной батареей:\n";
        h.FullInspection();
        h.GetSensor(2).Recharge();

        std::cout << "\nДинамическая память\n";
        greenhouse::Greenhouse* ptr = new greenhouse::Greenhouse("Теплица 2", &tarrif);
        ptr->FullInspection();
        delete ptr;
        ptr = nullptr;
        std::cout << "\nВыход из блока\n";
    }
    std::cout << "Проверка тарифа\n";
    tarrif.PrintInfo();
    
    std::cout << "\nДинамический массив объектов и Массив динамических объектов\n";
    //Динамический массив объектов
    greenhouse::Tariff* tariffArray = new greenhouse::Tariff[2]{
        greenhouse::Tariff("Новый1", 1.2),
        greenhouse::Tariff("Новый2", 1.3)
    };
    tariffArray[0].PrintInfo();
    tariffArray[1].PrintInfo();

    //Массив динамических объектов (массив указателей)
    greenhouse::Sensor* s[2] = {
        new greenhouse::Sensor("Динамич1", &tariffArray[0]),
        new greenhouse::Sensor("Динамич2", &tariffArray[1])
    };
    s[0]->TakeMeasurement();

    delete s[0];
    delete s[1];

    delete[] tariffArray;
    tariffArray = nullptr;

    return 0;
}