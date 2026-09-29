#pragma once

#include "sensor.hpp"
#include <string>
#include <string_view>
#include <array>

namespace greenhouse {

	class Greenhouse {
	private:
		std::string m_title{ "Теплица" };
		double m_targetTemperature{ 20.0 };
		std::array<Sensor*, 4> m_sensors{ nullptr, nullptr, nullptr, nullptr };

	public:
		Greenhouse() = default;
		Greenhouse(std::string_view title, Sensor* s1, Sensor* s2, Sensor* s3, Sensor* s4);
		~Greenhouse();

		void FullInspection();
		void PrintStatus() const;

		// Геттеры
		[[nodiscard]] std::string_view GetTitle() const { return m_title; }
	};

} // namespace greenhouse