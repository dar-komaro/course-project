#pragma once

#include "tariff.hpp"
#include "calibratable.hpp"
#include "measurement.hpp"
#include "exceptions.hpp"
#include <string>
#include <string_view>
#include <memory> 
#include <iostream>

namespace greenhouse {
	class BaseSensor {
	private:
		inline static int m_lastNumber{ 0 };
		int m_inventoryNumber{ 0 };
		std::string m_name{ "Универсальный" };
		std::weak_ptr<Tariff> m_tariff;
		std::unique_ptr<IMeasurementStrategy> m_strategy{ std::make_unique<StandardMode>() };
		[[nodiscard]] static int NextInventoryNumber() {
			return ++m_lastNumber;
		}

	public:
		BaseSensor();
		BaseSensor(std::string_view title, std::weak_ptr<Tariff> tariff);
		virtual ~BaseSensor() noexcept;
		virtual bool TakeMeasurement() = 0;
		virtual std::string PrintInfo() const;
		[[nodiscard]] virtual std::unique_ptr<BaseSensor> Clone() const = 0;
		void RegisterInTariff(Tariff& tariff) const;

		void Recharge();
		[[nodiscard]] bool IsKeywordInName(std::string_view keyword) const;
		[[nodiscard]] std::string_view GetName() const { return m_name; }
		[[nodiscard]] int GetBattery() const { return m_battery; }
		[[nodiscard]] int GetInventoryNumber() const { return m_inventoryNumber; }
		[[nodiscard]] std::string GetDeviceCategory() const {
			return "Базовый измерительный прибор";
		}
		// Метод для динам конфигурирования
		void SetStrategy(std::unique_ptr<IMeasurementStrategy> strategy) {
			if (strategy) {
				m_strategy = std::move(strategy);
				std::cout << "[Конфигурация] Датчик \"" << m_name
					<< "\" переключен в режим: " << m_strategy->GetModeName() << "\n";
			}
		}
		[[nodiscard]] std::string GetStrategyName() const {
			return m_strategy ? m_strategy->GetModeName() : "Нет";
		}
		
	protected:
		int m_battery{ 100 };
		BaseSensor(const BaseSensor& other);
		BaseSensor& operator=(const BaseSensor&) = delete;
		BaseSensor(BaseSensor&& other) noexcept = default;
		BaseSensor& operator=(BaseSensor&& other) noexcept = default;
		[[nodiscard]] std::string FormatBaseInfo() const {
			return "Датчик: " + m_name + " | Заряд: " + std::to_string(m_battery) + "%";
		}
		[[nodiscard]] const IMeasurementStrategy& GetStrategy() const { return *m_strategy; }
	};

	// 1. Датчик температуры 
	class TemperatureSensor : public BaseSensor {
	private:
		double m_currentTemp{ 22.0 };
		double m_targetTemp{ 24.0 };//Вводимое значение

	public:
		TemperatureSensor();
		TemperatureSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetTemp);
		~TemperatureSensor() noexcept override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override; 

		void SetTargetTemperature(double target) {
			if (target < 0.0 || target > 50.0) {
				throw std::invalid_argument("Целевая температура должна быть в диапазоне [0; 50] °C");
			}
			m_targetTemp = target;
		}
		[[nodiscard]] double GetTargetTemperature() const { return m_targetTemp; }
		[[nodiscard]] double GetCurrentTemperature() const { return m_currentTemp; }
		[[nodiscard]] std::string GetDeviceCategory() const {
			return "Датчик температуры";
		}
	};

	// 2. Датчик влажности
	class HumiditySensor : public BaseSensor {
	private:
		double m_currentHumidity{ 60.0 };
		double m_targetHumidity{ 65.0 };

	public:
		HumiditySensor();
		HumiditySensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetHumidity);
		~HumiditySensor() noexcept override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override;

		void SetTargetHumidity(double target) {
			if (target < 0.0 || target > 100.0) {
				throw std::invalid_argument("Целевая влажность должна быть в диапазоне [0; 100]%");
			}
			m_targetHumidity = target;
		}

		[[nodiscard]] double GetTargetHumidity() const { return m_targetHumidity; }
		[[nodiscard]] double GetCurrentHumidity() const { return m_currentHumidity; }
	};

	// 3. Датчик освещенности
	class LightSensor : public BaseSensor, public ICalibratable {
	private:
		double m_currentLight{ 5000.0 };
		double m_targetLight{ 6000.0 };
		bool m_isCalibrated{ false };

	public:
		LightSensor();
		LightSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetLight);
		~LightSensor() noexcept override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override;

		void SetTargetLight(double target) {
			if (target < 0.0 || target > 100000.0) {
				throw std::invalid_argument("Целевая освещенность должна быть в диапазоне [0; 100000] лк");
			}
			m_targetLight = target;
		}

		[[nodiscard]] double GetTargetLight() const { return m_targetLight; }
		[[nodiscard]] double GetCurrentLight() const { return m_currentLight; }
		void Calibrate(double offset) override;
		[[nodiscard]] bool IsCalibrated() const override { return m_isCalibrated; }
		[[nodiscard]] std::string GetDeviceCategory() const {
			return "Оптический датчик освещенности";
		}
	};

	// 4. Датчик удобрений (запрет дальнейшего наследования через final)
	class FertilizerSensor final : public BaseSensor, public ICalibratable {
	private:
		double m_currentFertilizer{ 50.0 };
		double m_targetFertilizer{ 60.0 };
		bool m_isCalibrated{ false };

	public:
		FertilizerSensor();
		FertilizerSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetFertilizer);
		~FertilizerSensor() noexcept override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override;

		void SetTargetFertilizer(double target) {
			if (target < 0.0 || target > 100.0) {
				throw std::invalid_argument("Целевой уровень удобрений должен быть в диапазоне [0; 100]%");
			}
			m_targetFertilizer = target;
		}

		[[nodiscard]] double GetTargetFertilizer() const { return m_targetFertilizer; }
		[[nodiscard]] double GetCurrentFertilizer() const { return m_currentFertilizer; }
		void Calibrate(double offset) override;
		[[nodiscard]] bool IsCalibrated() const override { return m_isCalibrated; }
	};
}