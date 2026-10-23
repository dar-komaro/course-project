#pragma once

#include "tariff.hpp"
#include <string>
#include <string_view>
#include <memory> 

namespace greenhouse {
	class Sensor {
	private:
		std::string m_name{ "Универсальный" };
		int m_battery{ 100 };
		std::weak_ptr<Tariff> m_tariff;

	public:
		Sensor() = default;
		Sensor(std::string_view title, std::weak_ptr<Tariff> tariff);
		~Sensor();
		bool TakeMeasurement();
		void Recharge();
		void PrintInfo() const;
		[[nodiscard]] std::string_view GetType() const { return m_name; }
		[[nodiscard]] int GetBattery() const { return m_battery; }
	};
}