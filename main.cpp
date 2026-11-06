#include "tariff.hpp"
#include "sensor.hpp"
#include "greenhouse.hpp"
#include <iostream>
#include <memory>

int main() {
    auto tariff = std::make_shared<greenhouse::Tariff>("Обычный", 1.25);
    {
        std::cout << "Вложенный блок\n";
        greenhouse::Greenhouse gHouse("Теплица 1", tariff);

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
        auto ptr = std::make_unique<greenhouse::Greenhouse>("Теплица 2", tariff);
        ptr->FullInspection();
        auto ptr2 = std::move(ptr);
        std::cout << "  ptr " << (ptr ? "не пуст" : "пуст") << "\n";
        std::cout << "  ptr2 " << (ptr2 ? "не пуст" : "пуст") << "\n";
        std::cout << "\nВыход из блока\n";
    }
    std::cout << "Проверка тарифа\n";
    tariff->PrintInfo();
    
    std::cout << "\nДинамический массив объектов и Массив динамических объектов\n";
    //Динамический массив объектов
    auto tariffArray = std::make_unique<greenhouse::Tariff[]>(2);
    tariffArray[0] = greenhouse::Tariff("Новый1", 1.2);
    tariffArray[1] = greenhouse::Tariff("Новый2", 1.3);
    tariffArray[0].PrintInfo();
    tariffArray[1].PrintInfo();

    //Массив динамических объектов
    std::unique_ptr<greenhouse::Sensor> s[2];
    s[0] = std::make_unique<greenhouse::Sensor>("Динамич1", tariff);
    s[1] = std::make_unique<greenhouse::Sensor>("Динамич2", tariff);
    s[0]->TakeMeasurement();

    std::cout << "\nСоздаём shared_ptr:\n";
    auto sp1 = std::make_shared<greenhouse::Tariff>("Общий", 5.0);
    std::cout << "sp1.use_count() = " << sp1.use_count() << "\n";

    auto sp2 = sp1;   // копия указателя
    std::cout << "sp2.use_count() = " << sp2.use_count() << "\n";

    std::cout << "\nУничтожаем sp2:\n";
    sp2.reset();
    std::cout << "sp1.use_count() = " << sp1.use_count() << "\n";
    sp1.reset();

    std::cout << "\nСоздаём shared_ptr и weak_ptr:\n";

    auto shared = std::make_shared<greenhouse::Tariff>("Наблюдаемый", 6.0);
    std::weak_ptr<greenhouse::Tariff> weak = shared;   // weak_ptr из shared_ptr

    std::cout << "shared.use_count() = " << shared.use_count() << "\n"; 
    std::cout << "weak.expired() = " << weak.expired() << "\n";
    if (auto locked = weak.lock()) {
        std::cout << "непустой shared_ptr\n";
        std::cout << "locked.use_count() = " << locked.use_count() << "\n";
    }
    else {
        std::cout << "пустой shared_ptr\n";
    }
    shared.reset();
    std::cout << "shared.use_count() = " << shared.use_count() << "\n";
    std::cout << "weak.expired() = " << weak.expired() << "\n";

    if (auto locked = weak.lock()) {
        std::cout << "непустой shared_ptr\n";
    }
    else {
        std::cout << "объект уничтожен\n";
    }

    return 0;
}