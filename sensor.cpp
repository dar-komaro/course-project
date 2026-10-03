#include "sensor.hpp"
#include <iostream>

namespace greenhouse {

	Sensor::Sensor(std::string_view title, const Tariff* tariff)
		: m_name{ title }
		, m_tariff{ tariff }
	{
		std::cout << "[+Sensor] Датчик \"" << m_name << "\" создан.\n";
	}

	Sensor::~Sensor() {
		std::cout << "[~Sensor] Датчик \"" << m_name << "\" удален.\n";
	}

	bool Sensor::TakeMeasurement() {
		if (m_battery <= 0) {
			std::cout << "[Ошибка " << m_name << "] Батарея разряжена (0%)!\n";
			return false;
		}

		// 5% заряда на замер
		m_battery -= 5;
		if (m_battery < 0) {
			m_battery = 0;
		}

		std::cout << "Замер выполнен (" << m_name << "). Остаток заряда: " << m_battery << "%\n";

		if (m_tariff != nullptr) {
			double cost = m_tariff->CalculateCost(1);
			std::cout << "Стоимость замера по тарифу \"" << m_tariff->GetName() << "\": " << cost << " руб.\n";
		}
		else {
			std::cout << "Тариф не привязан.\n";
		}

		return true;
	}

	//перезарядить батарею
	void Sensor::Recharge() {
		m_battery = 100;
		std::cout << "[Sensor] Датчик \"" << m_name << "\" заряжен до 100%.\n";
	}

	//информация о датчике
	void Sensor::PrintInfo() const {
		std::cout << "Датчик: " << m_name << " | Заряд: " << m_battery << "%";
		if (m_tariff != nullptr) {
			std::cout << " | Тариф: " << m_tariff->GetName();
		}
		else {
			std::cout << " | Тариф: [не назначен]";
		}
		std::cout << "\n";
	}

}