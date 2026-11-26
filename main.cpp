#include "tariff.hpp"
#include "sensor.hpp"
#include "greenhouse.hpp"
#include <iostream>
#include <memory>
#include <array>
#include <cstdlib>

// Функция работает с контрактом ICalibratable
void PerformCalibration(greenhouse::ICalibratable& device, double offset) {
    if (!device.IsCalibrated()) {
        device.Calibrate(offset);
    }
}

// Функция для проверки типа во время выполнения 
void ServiceSensor(greenhouse::BaseSensor& sensor) {
    std::cout << "Обслуживание: " << sensor.GetName() << " -> ";
    if (auto* calibratable = dynamic_cast<greenhouse::ICalibratable*>(&sensor)) {
        PerformCalibration(*calibratable, 1.5);
    }
    else {
        std::cout << "калибровка не требуется (не поддерживается).\n";
    }
}

int main (){
    auto sharedTariff = std::make_shared<greenhouse::Tariff>("Тариф1", 1.25);

    std::cout << "\nДемонстрация интерфейса ICalibratable и dynamic_cast\n";
    {
        std::array<std::unique_ptr<greenhouse::BaseSensor>, 4> sensors = {
            std::make_unique<greenhouse::TemperatureSensor>("Датчик Температуры Т-01", sharedTariff, 23.5),
            std::make_unique<greenhouse::HumiditySensor>("Датчик Влажности В-01", sharedTariff, 60.0),
            std::make_unique<greenhouse::LightSensor>("Датчик Освещенности О-01", sharedTariff, 4500.0),
            std::make_unique<greenhouse::FertilizerSensor>("Датчик Удобрений У-01", sharedTariff, 50.0)
        };

        for (const auto& sensor : sensors) {
            ServiceSensor(*sensor);
        }
    }

    std::cout << "\nДемонстрация раннего и позднего связывания\n";
    {
        std::unique_ptr<greenhouse::BaseSensor> ptr =
            std::make_unique<greenhouse::TemperatureSensor>("Датчик Т-99", sharedTariff, 25.0);

        // раннее связывание (метод GetDeviceCategory не виртуальный)
        // Компилятор смотрит только на тип указателя (BaseSensor*) и привязывает BaseSensor::GetDeviceCategory()
        std::cout << "Раннее связывание (GetDeviceCategory): " << ptr->GetDeviceCategory() << "\n";

        // поздее связывание (метод PrintInfo виртуал)
        // Во время работы программа смотрит в vtable реального объекта (TemperatureSensor) и вызывает TemperatureSensor::PrintInfo()
        std::cout << "Позднее связывание (PrintInfo):        " << ptr->PrintInfo() << "\n";
    }

    std::cout << "\nДемонстрация конфигурирования объекта поведения\n";
    {
        greenhouse::TemperatureSensor smartSensor("Климат-Контроль А1", sharedTariff, 24.0);

        // 1. Замер в стандартном режиме (по умолчанию расход 5%)
        smartSensor.TakeMeasurement();

        // 2. Динамически меняем объект поведения на Эко-режим (теперь расход 1%)
        smartSensor.SetStrategy(std::make_unique<greenhouse::EcoMode>());
        smartSensor.TakeMeasurement();
    }

    std::cout << "\nДемонстрация исключений\n";
    {
        greenhouse::LightSensor lightSensor("Свет-Юг", sharedTariff, 5000.0);

        // Сценарий 1: Разряд батареи и восстановление (Recharge)
        try {
            std::cout << "Запускаем серию замеров до разряда батареи...\n";
            for (int i = 0; i < 25; ++i) { // Заряда 100% хватит на 20 замеров (по 5%)
                lightSensor.TakeMeasurement();
            }
        }
        catch (const greenhouse::LowBatteryException& e) { // Частный тип — первым!
            std::cout << "[Перехвачено исключение] " << e.what() << "\n";
            std::cout << " -> Действие системы: отправляем датчик на подзарядку!\n";
            lightSensor.Recharge();
            lightSensor.TakeMeasurement(); // Повторный замер после зарядки успешен!
        }
        catch (const std::exception& e) { // Общий тип — последним!
            std::cout << "[Общая ошибка] " << e.what() << "\n";
        }

        // Сценарий 2: Ошибка калибровки и некорректная уставка
        try {
            std::cout << "\nПробуем откалибровать с недопустимой поправкой 500.0...\n";
            lightSensor.Calibrate(500.0);
        }
        catch (const greenhouse::InvalidCalibrationException& e) { // Частный тип
            std::cout << "[Перехвачено InvalidCalibrationException] " << e.what() << "\n";
        }
        catch (const greenhouse::GreenhouseException& e) { // Базовый класс теплицы
            std::cout << "[Перехвачено GreenhouseException] " << e.what() << "\n";
        }
        catch (const std::exception& e) { // Стандартный базовый класс
            std::cout << "[Перехвачено std::exception] " << e.what() << "\n";
        }
    }

}
