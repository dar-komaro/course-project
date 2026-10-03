#pragma once

#include "tariff.hpp"
#include <string>
#include <string_view>

namespace greenhouse {
	class Sensor {
	private:
		std::string m_name{ "Универсальный" };
		int m_battery{ 100 };
		const Tariff* m_tariff{ nullptr };

	public:
		Sensor() = default;
		Sensor(std::string_view title, const Tariff* tariff);
		~Sensor();
		bool TakeMeasurement();
		void Recharge();
		void PrintInfo() const;
		[[nodiscard]] std::string_view GetType() const { return m_name; }
		[[nodiscard]] int GetBattery() const { return m_battery; }
	};
}