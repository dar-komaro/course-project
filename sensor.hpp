#pragma once

#include "tariff.hpp"
#include <string>
#include <string_view>
#include <memory> 

namespace greenhouse {
	class BaseSensor {
	private:
		inline static int m_lastNumber{ 0 };
		int m_inventoryNumber{ 0 };
		std::string m_name{ "Универсальный" };
		std::weak_ptr<Tariff> m_tariff;
		[[nodiscard]] static int NextInventoryNumber() {
			return ++m_lastNumber;
		}

	public:
		BaseSensor();
		BaseSensor(std::string_view title, std::weak_ptr<Tariff> tariff);
		virtual ~BaseSensor();
		virtual bool TakeMeasurement() = 0;
		virtual std::string PrintInfo() const;
		[[nodiscard]] virtual std::unique_ptr<BaseSensor> Clone() const = 0;
		void RegisterInTariff(Tariff& tariff) const;

		void Recharge();
		[[nodiscard]] bool IsKeywordInName(std::string_view keyword) const;
		[[nodiscard]] std::string_view GetName() const { return m_name; }
		[[nodiscard]] int GetBattery() const { return m_battery; }
		[[nodiscard]] int GetInventoryNumber() const { return m_inventoryNumber; }
		
	protected:
		int m_battery{ 100 };
		BaseSensor(const BaseSensor& other) = default;
		BaseSensor& operator=(const BaseSensor& other) = default;
		BaseSensor(BaseSensor&& other) noexcept = default;
		BaseSensor& operator=(BaseSensor&& other) noexcept = default;
		[[nodiscard]] std::string FormatBaseInfo() const {
			return "Датчик: " + m_name + " | Заряд: " + std::to_string(m_battery) + "%";
		}
	};

	// 1. Датчик температуры 
	class TemperatureSensor : public BaseSensor {
	private:
		double m_currentTemp{ 22.0 };
		double m_targetTemp{ 24.0 };//Вводимое значение

	public:
		TemperatureSensor();
		TemperatureSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetTemp);
		~TemperatureSensor() override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override; 

		void SetTargetTemperature(double target) {
			if (target >= 0.0 && target <= 50.0) {
				m_targetTemp = target;
			}
		}
		[[nodiscard]] double GetTargetTemperature() const { return m_targetTemp; }
		[[nodiscard]] double GetCurrentTemperature() const { return m_currentTemp; }
	};

	// 2. Датчик влажности
	class HumiditySensor : public BaseSensor {
	private:
		double m_currentHumidity{ 60.0 };
		double m_targetHumidity{ 65.0 };

	public:
		HumiditySensor();
		HumiditySensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetHumidity);
		~HumiditySensor() override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override;

		void SetTargetHumidity(double target) {
			if (target >= 0.0 && target <= 100.0) {
				m_targetHumidity = target;
			}
		}
		[[nodiscard]] double GetTargetHumidity() const { return m_targetHumidity; }
		[[nodiscard]] double GetCurrentHumidity() const { return m_currentHumidity; }
	};

	// 3. Датчик освещенности (переопределение PrintInfo БЕЗ вызова базы)
	class LightSensor : public BaseSensor {
	private:
		double m_currentLight{ 5000.0 };
		double m_targetLight{ 6000.0 };

	public:
		LightSensor();
		LightSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetLight);
		~LightSensor() override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override;

		void SetTargetLight(double target) {
			if (target >= 0.0 && target <= 100000.0) {
				m_targetLight = target;
			}
		}
		[[nodiscard]] double GetTargetLight() const { return m_targetLight; }
		[[nodiscard]] double GetCurrentLight() const { return m_currentLight; }
	};

	// 4. Датчик удобрений (запрет дальнейшего наследования через final)
	class FertilizerSensor final : public BaseSensor {
	private:
		double m_currentFertilizer{ 50.0 };
		double m_targetFertilizer{ 60.0 };

	public:
		FertilizerSensor();
		FertilizerSensor(std::string_view title, std::weak_ptr<Tariff> tariff, double targetFertilizer);
		~FertilizerSensor() override;

		bool TakeMeasurement() override;
		[[nodiscard]] std::unique_ptr<BaseSensor> Clone() const override;
		std::string PrintInfo() const override;

		void SetTargetFertilizer(double target) {
			if (target >= 0.0 && target <= 100.0) {
				m_targetFertilizer = target;
			}
		}
		[[nodiscard]] double GetTargetFertilizer() const { return m_targetFertilizer; }
		[[nodiscard]] double GetCurrentFertilizer() const { return m_currentFertilizer; }
	};
}