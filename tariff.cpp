#include "tariff.hpp"
#include <iostream>
#include "sensor.hpp" 
#include <stdexcept>

namespace greenhouse {
	Tariff::Tariff(std::string_view name, double price)
		: m_name{name}
		, m_priceUnit{price <0.0 ? 0.0 :price}
	{
		if (price < 0.0) {
			throw std::invalid_argument("Цена тарифа не может быть отрицательной");
		}
		std::cout << "[+Tariff] Тариф \"" << m_name << "\" создан (Цена: " << m_priceUnit << " руб).\n";
	}
	Tariff::~Tariff() noexcept { std::cout << "[~Tariff] Тариф \"" << m_name << "\" удален.\n"; }

	double Tariff::CalculateCost(int count) const {
		if (count <= 0) { return 0.0; }
		return m_priceUnit * count;
	}

	void Tariff::UpdatePrice(double newPrice) {
		if (newPrice < 0.0) {
			throw std::invalid_argument("Отрицательная цена тарифа недопустима");
		}
		m_priceUnit = newPrice;
	}

	void Tariff::PrintInfo() const {
		std::cout<< "Тариф: " << m_name << " | Цена за единицу: " << m_priceUnit << " руб.\n";
	}

	void Tariff::LogSensorRegistration(const BaseSensor& sensor) {
		std::cout << "[Тариф] К плану успешно привязан " << sensor.GetName() << "\n";
	}
}