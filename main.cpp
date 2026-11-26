#include "tariff.hpp"
#include "sensor.hpp"
#include "greenhouse.hpp"
#include <iostream>
#include <memory>
#include <array>
#include <cstdlib>

int main() {
    auto sharedTariff = std::make_shared<greenhouse::Tariff>("Тариф1", 1.25);
    std::cout << "\n";


    std::cout << "Массив\n";
    {
        // Создаем массив датчиков разных типов, управляемый указателями на BaseSensor
        std::array<std::unique_ptr<greenhouse::BaseSensor>, 4> arr = {
            std::make_unique<greenhouse::TemperatureSensor>("Датчик Температуры Т-01", sharedTariff, 23.5),
            std::make_unique<greenhouse::HumiditySensor>("Датчик Влажности В-01", sharedTariff, 60.0),
            std::make_unique<greenhouse::LightSensor>("Датчик Освещенности О-01", sharedTariff, 4500.0),
            std::make_unique<greenhouse::FertilizerSensor>("Датчик Удобрений У-01", sharedTariff, 50.0)
        };

        std::cout << "\nВызов функций:\n";
        for (const auto& sensor : arr) {
            sensor->TakeMeasurement(); // Виртуальный метод замера
            std::cout << "  Инфо: " << sensor->PrintInfo() << "\n\n"; // Виртуальный метод вывода
        }

        std::cout << "Выход из блока. Автоматическое удаление массива:\n";
    }
    std::cout << "\n";

    std::cout << "Демонстрация работы со строками\n";
    {
        greenhouse::TemperatureSensor testSensor("Центральный Датчик Климата", sharedTariff, 22.0);

        //сложение строк
        std::string r = "Отчет системы: " + testSensor.PrintInfo();
        std::cout << "  Сложение: " << r << "\n";

        // Поиск выделение и сравнение
        std::string_view targetKeyword = "Климата";
        std::cout << "  Поиск подстроки '" << targetKeyword << "' в имени датчика\n";
        if (testSensor.IsKeywordInName(targetKeyword)) {
            std::cout << "Подстрока найдена\n";
        }
    }
    std::cout << "\n";

    std::cout << "Использование указателя 'this'\n";
    {
        greenhouse::HumiditySensor singleSensor("Влажность-Тест", sharedTariff, 55.0);
        singleSensor.RegisterInTariff(*sharedTariff);
    }
    std::cout << "\n";


    std::cout << "Поверхностное и Глубокое копирование\n";
    {
        std::cout << "Поверхностное копирование\n";
        auto originalTariff = std::make_shared<greenhouse::Tariff>("Базовый", 10.0);

        auto shallowCopyTariff = originalTariff;

        std::cout << "Цена оригинала: " << originalTariff->GetPriceUnit() << " руб."
            << " | Цена копии: " << shallowCopyTariff->GetPriceUnit() << " руб.\n";

        // Меняем цену в копии
        std::cout << " Меняем цену в поверхностной копии на 99.9... *\n";
        shallowCopyTariff->UpdatePrice(99.9);

        // Проверяем
        std::cout << "Цена оригинала: " << originalTariff->GetPriceUnit() << " руб."
            << " | Цена копии: " << shallowCopyTariff->GetPriceUnit() << " руб.\n\n";


        std::cout << "Глубокое копирование\n";
        // Создаем оригинальный датчик температуры
        greenhouse::TemperatureSensor originalSensor("T1", originalTariff, 22.0);
        originalSensor.TakeMeasurement();

        std::unique_ptr<greenhouse::BaseSensor> sens = originalSensor.Clone();

        std::cout << "Оригинал: " << originalSensor.PrintInfo() << "\n";
        std::cout << "Клон:     " << sens->PrintInfo() << "\n";

        // Изменяем состояние копии
        std::cout << " Делаем замер только на Глубоком клоне*\n";
        sens->TakeMeasurement();

        // Проверяем
        std::cout << "Оригинал: " << originalSensor.PrintInfo() << "\n";
        std::cout << "Клон:     " << sens->PrintInfo() << "\n";
    }
    std::cout << "\n";


    {
        // Инициализируем теплицу, которая внутри содержит массив датчиков
        greenhouse::Greenhouse mainGreenhouse("Теплица1", sharedTariff);

        std::cout << "\nМониторинг теплицы\n";
        mainGreenhouse.PrintStatus();

        std::cout << "\nОпрос датчиков\n";
        mainGreenhouse.FullInspection();

        std::cout << "\nСтатус теплицы после изменения параметров\n";
        mainGreenhouse.PrintStatus();

    }

    return 0;
}

