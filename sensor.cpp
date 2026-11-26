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

        BaseSensor::BaseSensor(const BaseSensor& other)
            : m_inventoryNumber(NextInventoryNumber())
            , m_name(other.m_name)
            , m_tariff(other.m_tariff)
            , m_battery(other.m_battery)
            , m_strategy(std::make_unique<StandardMode>())
        {
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

        TemperatureSensor::~TemperatureSensor() noexcept {
            std::cout << "[~TemperatureSensor] Уничтожен температурный датчик\n";
        }

        bool TemperatureSensor::TakeMeasurement() {
            int drain = GetStrategy().GetBatteryDrain();
            if (m_battery < drain) {
                throw LowBatteryException(GetName(), m_battery);
            }
            m_battery -= drain;
            m_currentTemp = GetStrategy().ComputeValue(m_targetTemp);

            std::cout << "[Замер Темп | Режим: " << GetStrategyName() << "] " << GetName()
                << " замерил: " << m_currentTemp << " °C (остаток заряда: " << m_battery << "%)\n";
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

        HumiditySensor::~HumiditySensor() noexcept {
            std::cout << "[~HumiditySensor] Уничтожен датчик влажности\n";
        }

        bool HumiditySensor::TakeMeasurement() {
            int drain = GetStrategy().GetBatteryDrain();
            if (m_battery < drain) {
                throw LowBatteryException(GetName(), m_battery);
            }
            m_battery -= drain;
            m_currentHumidity = GetStrategy().ComputeValue(m_targetHumidity);

            std::cout << "[Замер Влажности | Режим: " << GetStrategyName() << "] " << GetName()
                << " замерил: " << m_currentHumidity << "% (остаток заряда: " << m_battery << "%)\n";
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

        LightSensor::~LightSensor() noexcept {
            std::cout << "[~LightSensor] Уничтожен датчик освещенности\n";
        }

        bool LightSensor::TakeMeasurement() {
            int drain = GetStrategy().GetBatteryDrain();
            if (m_battery < drain) {
                throw LowBatteryException(GetName(), m_battery);
            }
            m_battery -= drain;

            // Вычисляем базовое отклонение через стратегию и масштабируем его под люксы (x50)
            double deviation = GetStrategy().ComputeValue(m_targetLight) - m_targetLight;
            m_currentLight = m_targetLight + deviation * 50.0;

            std::cout << "[Замер Света | Режим: " << GetStrategyName() << "] " << GetName()
                << " замерил: " << m_currentLight << " лк (остаток заряда: " << m_battery << "%)\n";
            return true;
        }


        std::unique_ptr<BaseSensor> LightSensor::Clone() const {
            return std::make_unique<LightSensor>(*this);
        }

        std::string LightSensor::PrintInfo() const {
            return "Световой прибор \"" + std::string(GetName()) + "\" | Заряд: " + std::to_string(GetBattery())
                + "% | Свет: " + std::to_string(m_currentLight) + " / " + std::to_string(m_targetLight) + " лк";
        }

        void LightSensor::Calibrate(double offset) {
            if (offset < -100.0 || offset > 100.0) {
                throw InvalidCalibrationException(GetName(), offset);
            }
            m_currentLight += offset;
            m_isCalibrated = true;
            std::cout << "[Калибровка] Датчик света \"" << GetName()
                << "\" откалиброван (поправка: " << offset << " лк).\n";
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

        FertilizerSensor::~FertilizerSensor() noexcept {
            std::cout << "[~FertilizerSensor] Уничтожен датчик удобрений\n";
        }

        bool FertilizerSensor::TakeMeasurement() {
            int drain = GetStrategy().GetBatteryDrain();
            if (m_battery < drain) {
                throw LowBatteryException(GetName(), m_battery);
            }
            m_battery -= drain;
            m_currentFertilizer = GetStrategy().ComputeValue(m_targetFertilizer);

            std::cout << "[Замер Удобрений | Режим: " << GetStrategyName() << "] " << GetName()
                << " замерил: " << m_currentFertilizer << "% (остаток заряда: " << m_battery << "%)\n";
            return true;
        }


        std::unique_ptr<BaseSensor> FertilizerSensor::Clone() const {
            return std::make_unique<FertilizerSensor>(*this);
        }

        std::string FertilizerSensor::PrintInfo() const {
            return BaseSensor::PrintInfo() + " | Удобрения: факт = " + std::to_string(m_currentFertilizer)
                + "% (Цель: " + std::to_string(m_targetFertilizer) + "%)";
        }

        void FertilizerSensor::Calibrate(double offset) {
            if (offset < -10.0 || offset > 10.0) {
                throw InvalidCalibrationException(GetName(), offset);
            }
            m_currentFertilizer += offset;
            m_isCalibrated = true;
            std::cout << "[Калибровка] Датчик удобрений \"" << GetName()
                << "\" откалиброван (поправка: " << offset << "%).\n";
        }

    } // namespace greenhouse
