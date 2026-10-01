#pragma once

#include "tariff.hpp" 
#include "sensor.hpp"
#include <string>
#include <string_view>
#include <array>

namespace greenhouse {

	class Greenhouse {
	private:
		std::string m_title{ "Теплица" };
		double m_temperature{ 20.0 };
		std::array<greenhouse::Sensor, 4> m_sensors;

	public:
		Greenhouse() = default;
		Greenhouse(std::string_view title, greenhouse::Tariff *tariff);
		~Greenhouse();

		void FullInspection();
		void PrintStatus() const;

		// Геттер
		greenhouse::Sensor& GetSensor(size_t index) { return m_sensors[index]; }
	};

} // namespace greenhouse