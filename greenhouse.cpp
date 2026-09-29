#include "greenhouse.hpp"
#include <iostream>

namespace greenhouse {

	Greenhouse::Greenhouse(std::string_view title, Sensor* s1, Sensor* s2, Sensor* s3, Sensor* s4)
		: m_title{ title }
		, m_sensors{ s1, s2, s3, s4 }
	{

		for (size_t i = 0; i < m_sensors.size(); ++i) {
			if (m_sensors[i] == nullptr) {
				std::cout << "[Ошибка " << m_title << "] Датчик №" << (i + 1) << " не подключен!\n";
			}
		}
	}

	Greenhouse::~Greenhouse() {
		std::cout << "[~Greenhouse] Теплица \"" << m_title << "\" удалена.\n";
	}

	void Greenhouse::FullInspection() {
		std::cout << "\n--- Запуск опроса 4-х датчиков теплицы \"" << m_title << "\" ---\n";

		for (size_t i = 0; i < m_sensors.size(); ++i) {
			if (m_sensors[i] != nullptr) {
				m_sensors[i]->TakeMeasurement();
			}
			else {
				std::cout << " [!] Датчик №" << (i + 1) << " не существует.\n";
			}
		}
	}

	void Greenhouse::PrintStatus() const {
		std::cout << "Теплица: " << m_title << " | Статус: укомплектована (4 датчика)\n";
	}

} // namespace greenhouse