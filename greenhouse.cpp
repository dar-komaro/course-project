#include "greenhouse.hpp"
#include <iostream>

namespace greenhouse {

	Greenhouse::Greenhouse(std::string_view title, greenhouse::Tariff* tariff)
		: m_title{ title }
		, m_sensors{
			greenhouse::Sensor("D1", tariff),
			greenhouse::Sensor("D2", tariff),
			greenhouse::Sensor("D3", tariff),
			greenhouse::Sensor("D4", tariff)
		}

	{
		std::cout << "[+Greenhouse] Теплица \"" << m_title << "\" создана.\n";
	}

	Greenhouse::~Greenhouse() {
		std::cout << "[~Greenhouse] Теплица \"" << m_title << "\" удалена.\n";
	}

	void Greenhouse::FullInspection() {
		std::cout << "\n--- Запуск опроса 4-х датчиков теплицы \"" << m_title << "\" ---\n";
		for (size_t i = 0; i < m_sensors.size(); ++i) {
			m_sensors[i].TakeMeasurement();

		}
	}

	void Greenhouse::PrintStatus() const {
		std::cout << "Теплица: " << m_title << " | Температура: " << m_temperature << "°C\n";
	}

} // namespace greenhouse