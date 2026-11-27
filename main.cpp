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
        std::cout << "Раннее связывание (GetDeviceCategory): " << ptr->GetDeviceCategory() << "\n";

        // поздее связывание (метод PrintInfo виртуал)
        std::cout << "Позднее связывание (PrintInfo):        " << ptr->PrintInfo() << "\n";
    }

    std::cout << "\nДемонстрация конфигурирования объекта поведения\n";
    {
        greenhouse::TemperatureSensor smartSensor("Климат-Контроль А1", sharedTariff, 24.0);

        // Замер в стандартном режиме (по умолчанию расход 5%)
        smartSensor.TakeMeasurement();

        // меняем объект поведения на Эко-режим (теперь расход 1%)
        smartSensor.SetStrategy(std::make_unique<greenhouse::EcoMode>());
        smartSensor.TakeMeasurement();
    }

    std::cout << "\nДемонстрация исключений\n";
    {
        greenhouse::LightSensor lightSensor("Свет-Юг", sharedTariff, 5000.0);

        // Разряд батареи и восстановление 
        try {
            std::cout << "Запускаем серию замеров до разряда батареи...\n";
            for (int i = 0; i < 25; ++i) { 
                lightSensor.TakeMeasurement();
            }
        }
        catch (const greenhouse::LowBatteryException& e) { 
            std::cout << "[Перехвачено исключение] " << e.what() << "\n";
            std::cout << " -> Действие системы: отправляем датчик на подзарядку!\n";
            lightSensor.Recharge();
            lightSensor.TakeMeasurement(); 
        }
        catch (const std::exception& e) {
            std::cout << "[Общая ошибка] " << e.what() << "\n";
        }

        //  Ошибка калибровки и некорректная уставка
        try {
            std::cout << "\nПробуем откалибровать с недопустимой поправкой 500.0...\n";
            lightSensor.Calibrate(500.0);
        }
        catch (const greenhouse::InvalidCalibrationException& e) { 
            std::cout << "[Перехвачено InvalidCalibrationException] " << e.what() << "\n";
        }
        catch (const greenhouse::GreenhouseException& e) {
            std::cout << "[Перехвачено GreenhouseException] " << e.what() << "\n";
        }
        catch (const std::exception& e) { 
            std::cout << "[Перехвачено std::exception] " << e.what() << "\n";
        }
    }

}
