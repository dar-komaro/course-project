#include "tariff.hpp"
#include <iostream>

namespace greenhouse {
	Tariff::Tariff(std::string_view name, double price)
		: m_name{name}
		, m_priceUnit{price <0.0 ? 0.0 :price}
	{ }
	Tariff::~Tariff() { std::cout << "[~Tariff] Тариф \"" << m_name << "\" удален.\n"; }

	double Tariff::CalculateCost(int count) const {
		if (count <= 0) { return 0.0; }
		return m_priceUnit * count;
	}

	bool Tariff::UpdatePrice(double newPrice) {
		if (newPrice < 0.0) {
			std::cout << "[Ошибка Tariff] Отрицательная цена не допустима!\n";
			return false;
		}
		m_priceUnit = newPrice;
		return true;
	}

	void Tariff::PrintInfo() const {
		std::cout<< "Тариф: " << m_name << " | Цена за единицу: " << m_priceUnit << " руб.\n";
	}
}