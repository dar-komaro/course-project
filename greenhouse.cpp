#include "greenhouse.hpp"
#include <iostream>

namespace greenhouse {

	Greenhouse::Greenhouse(std::string_view title, std::weak_ptr<Tariff> tariff)
		: m_title{ title }
		, m_sensors{
			std::make_unique<greenhouse::TemperatureSensor>("Датчик Температуры T1", tariff, 24.0),
			std::make_unique<greenhouse::HumiditySensor>("Датчик Влажности H1", tariff, 65.0),
			std::make_unique<greenhouse::LightSensor>("Датчик Освещенности L1", tariff, 5000.0),
			std::make_unique<greenhouse::FertilizerSensor>("Датчик Удобрений F1", tariff, 60.0)
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
			m_sensors[i]->TakeMeasurement();

		}
	}

	void Greenhouse::PrintStatus() const {
		std::cout << "Теплица: " << m_title << "\n";
		std::cout << "Состояние установленных приборов:\n";
		for (const auto& sensor : m_sensors) {
			std::cout << "  -> " << sensor->PrintInfo() << "\n";
		}
	}

} // namespace greenhouse