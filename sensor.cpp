#include "sensor.h"
#include <iostream>

namespace greenhouse {

	Sensor::Sensor(std::string_view type, const Tariff* tariff)
		: m_type{ type }
		, m_tariff{ tariff }
	{
	}

	Sensor::~Sensor() {
		std::cout << "[~Sensor] Датчик \"" << m_type << "\" удален.\n";
	}

	bool Sensor::TakeMeasurement() {
		if (m_battery <= 0) {
			std::cout << "[Ошибка " << m_type << "] Батарея разряжена (0%)!\n";
			return false;
		}

		// 5% заряда на замер
		m_battery -= 5;
		if (m_battery < 0) {
			m_battery = 0;
		}

		std::cout << "Замер выполнен (" << m_type << "). Остаток заряда: " << m_battery << "%\n";

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
		std::cout << "[Sensor] Датчик \"" << m_type << "\" заряжен до 100%.\n";
	}

	//информация о датчике
	void Sensor::PrintInfo() const {
		std::cout << "Датчик: " << m_type << " | Заряд: " << m_battery << "%";
		if (m_tariff != nullptr) {
			std::cout << " | Тариф: " << m_tariff->GetName();
		}
		else {
			std::cout << " | Тариф: [не назначен]";
		}
		std::cout << "\n";
	}

}