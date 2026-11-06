#include "sensor.hpp"
#include <iostream>

namespace greenhouse {

	Sensor::Sensor(std::string_view title, std::weak_ptr<Tariff> tariff)
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

		if (auto t = m_tariff.lock()) {
			double cost = t->CalculateCost(1);
			std::cout << "Стоимость замера по тарифу \"" << t->GetName() << "\": " << cost << " руб.\n";
		}
		else {
			std::cout << "Тариф уничтожен.\n";
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
		if (auto t = m_tariff.lock()) {
			std::cout << " | Тариф: " << t->GetName();
		}
		else {
			std::cout << " | Тариф: [уничтожен]";
		}
		std::cout << "\n";
	}
}