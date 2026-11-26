#include "tariff.hpp"
#include <cstdlib>
#include "sensor.hpp"
#include <iostream>

    namespace greenhouse {

        // BaseSensor

        BaseSensor::BaseSensor()
            : m_inventoryNumber(NextInventoryNumber())
        {
            std::cout << "[+BaseSensor] Создан базовый датчик по умолчанию | Инв. №: #" << m_inventoryNumber << "\n";
        }

        BaseSensor::BaseSensor(std::string_view title, std::weak_ptr<Tariff> tariff)
            : m_inventoryNumber(NextInventoryNumber()),
            m_name(title),
            m_battery(100),
            m_tariff(tariff)
        {
            std::cout << "[+BaseSensor] Датчик: \"" << m_name
                << "\" | Инвентарный номер: #" << m_inventoryNumber << "\n";
        }

        BaseSensor::~BaseSensor() {
            std::cout << "[~BaseSensor] Датчик: \"" << m_name
                << "\" | Инвентарный номер: #" << m_inventoryNumber << "\n";
        }

        void BaseSensor::Recharge() {
            m_battery = 100;
            std::cout << "[Sensor] Датчик #" << m_inventoryNumber << " (" << m_name << ") полностью перезаряжен (100%).\n";
        }

        std::string BaseSensor::PrintInfo() const {
            return FormatBaseInfo() + " | Инв. №: " + std::to_string(m_inventoryNumber);
        }

        void BaseSensor::RegisterInTariff(Tariff& tariff) const {
            // Датчик вызывает метод тарифа и передает ему СЕБЯ (через *this)
            tariff.LogSensorRegistration(*this);
        }

        bool BaseSensor::IsKeywordInName(std::string_view keyword) const {
            if (m_name == keyword) { // Сравнение строк
                return true;
            }
            size_t pos = m_name.find(keyword); // Поиск подстроки
            if (pos != std::string::npos) {
                std::string sub = m_name.substr(pos, keyword.size()); // Выделение подстроки
                return sub == keyword;
            }
            return false;
        }


        // TemperatureSensor

        TemperatureSensor::TemperatureSensor()
            : BaseSensor()
        {
            std::cout << "[+TemperatureSensor] Создан датчик температуры по умолчанию\n";
        }

        TemperatureSensor::TemperatureSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetTemp)
            : BaseSensor(title, tariff), m_targetTemp(targetTemp)
        {
            std::cout << "[+TemperatureSensor] Инициализирован температурный датчик \"" << title << "\"\n";
        }

        TemperatureSensor::~TemperatureSensor() {
            std::cout << "[~TemperatureSensor] Уничтожен температурный датчик\n";
        }

        bool TemperatureSensor::TakeMeasurement() {
            if (m_battery <= 0) {
                std::cout << "[Ошибка] Датчик температуры #" << GetInventoryNumber() << " разряжен!\n";
                return false;
            }
            m_battery -= 5;
            m_currentTemp = m_targetTemp + (rand() % 5 - 2) * 0.5;
            std::cout << "[Замер Темп] " << GetName() << " замерил температуру: факт = " << m_currentTemp
                << " °C (Цель: " << m_targetTemp << " °C)\n";
            return true;
        }

        std::unique_ptr<BaseSensor> TemperatureSensor::Clone() const {
            return std::make_unique<TemperatureSensor>(*this);
        }

        std::string TemperatureSensor::PrintInfo() const {
            return BaseSensor::PrintInfo() + " | Температура: факт = " + std::to_string(m_currentTemp)
                + " °C (Цель: " + std::to_string(m_targetTemp) + " °C)";
        }


        // --- 2. HumiditySensor ---

        HumiditySensor::HumiditySensor()
            : BaseSensor()
        {
            std::cout << "[+HumiditySensor] Создан датчик влажности по умолчанию\n";
        }

        HumiditySensor::HumiditySensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetHumidity)
            : BaseSensor(title, tariff), m_targetHumidity(targetHumidity)
        {
            std::cout << "[+HumiditySensor] Инициализирован датчик влажности \"" << title << "\"\n";
        }

        HumiditySensor::~HumiditySensor() {
            std::cout << "[~HumiditySensor] Уничтожен датчик влажности\n";
        }

        bool HumiditySensor::TakeMeasurement() {
            if (m_battery <= 0) {
                std::cout << "[Ошибка] Датчик влажности #" << GetInventoryNumber() << " разряжен!\n";
                return false;
            }
            m_battery -= 4;
            m_currentHumidity = m_targetHumidity + (rand() % 10 - 5) * 0.8;
            std::cout << "[Замер Влажности] " << GetName() << " замерил влажность: факт = " << m_currentHumidity
                << "% (Цель: " << m_targetHumidity << "%)\n";
            return true;
        }

        std::unique_ptr<BaseSensor> HumiditySensor::Clone() const {
            return std::make_unique<HumiditySensor>(*this);
        }

        std::string HumiditySensor::PrintInfo() const {
            return BaseSensor::PrintInfo() + " | Влажность: факт = " + std::to_string(m_currentHumidity)
                + "% (Цель: " + std::to_string(m_targetHumidity) + "%)";
        }


        // LightSensor

        LightSensor::LightSensor()
            : BaseSensor()
        {
            std::cout << "[+LightSensor] Создан датчик освещенности по умолчанию\n";
        }

        LightSensor::LightSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetLight)
            : BaseSensor(title, tariff), m_targetLight(targetLight)
        {
            std::cout << "[+LightSensor] Инициализирован датчик освещенности \"" << title << "\"\n";
        }

        LightSensor::~LightSensor() {
            std::cout << "[~LightSensor] Уничтожен датчик освещенности\n";
        }

        bool LightSensor::TakeMeasurement() {
            if (m_battery <= 0) {
                std::cout << "[Ошибка] Датчик освещенности #" << GetInventoryNumber() << " разряжен!\n";
                return false;
            }
            m_battery -= 6;
            m_currentLight = m_targetLight + (rand() % 200 - 100);
            std::cout << "[Замер Света] " << GetName() << " замерил свет: факт = " << m_currentLight
                << " лк (Цель: " << m_targetLight << " лк)\n";
            return true;
        }

        std::unique_ptr<BaseSensor> LightSensor::Clone() const {
            return std::make_unique<LightSensor>(*this);
        }

        std::string LightSensor::PrintInfo() const {
            return "Световой прибор \"" + std::string(GetName()) + "\" | Заряд: " + std::to_string(GetBattery())
                + "% | Свет: " + std::to_string(m_currentLight) + " / " + std::to_string(m_targetLight) + " лк";
        }


        // FertilizerSensor

        FertilizerSensor::FertilizerSensor()
            : BaseSensor()
        {
            std::cout << "[+FertilizerSensor] Создан датчик удобрений по умолчанию\n";
        }

        FertilizerSensor::FertilizerSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetFertilizer)
            : BaseSensor(title, tariff), m_targetFertilizer(targetFertilizer)
        {
            std::cout << "[+FertilizerSensor] Инициализирован датчик удобрений \"" << title << "\"\n";
        }

        FertilizerSensor::~FertilizerSensor() {
            std::cout << "[~FertilizerSensor] Уничтожен датчик удобрений\n";
        }

        bool FertilizerSensor::TakeMeasurement() {
            if (m_battery <= 0) {
                std::cout << "[Ошибка] Датчик удобрений #" << GetInventoryNumber() << " разряжен!\n";
                return false;
            }
            m_battery -= 3;
            m_currentFertilizer = m_targetFertilizer + (rand() % 6 - 3) * 0.5;
            std::cout << "[Замер Удобрений] " << GetName() << " замерил уровень: факт = " << m_currentFertilizer
                << "% (Цель: " << m_targetFertilizer << "%)\n";
            return true;
        }

        std::unique_ptr<BaseSensor> FertilizerSensor::Clone() const {
            return std::make_unique<FertilizerSensor>(*this);
        }

        std::string FertilizerSensor::PrintInfo() const {
            return BaseSensor::PrintInfo() + " | Удобрения: факт = " + std::to_string(m_currentFertilizer)
                + "% (Цель: " + std::to_string(m_targetFertilizer) + "%)";
        }

    } // namespace greenhouse
