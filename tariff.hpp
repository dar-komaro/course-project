#pragma once

#include <string>
#include <string_view>

namespace greenhouse {

	class Tariff {
	private:
		std::string m_name{ "Стандартный" };
		double m_priceUnit{ 0.0 };

	public:
		Tariff() = default;
		Tariff(std::string_view name, double price);
		~Tariff();

		[[nodiscard]] double CalculateCost(int count) const;
		bool UpdatePrice(double newPrice);
		void PrintInfo() const;

		[[nodiscard]] std::string_view GetName() const { return m_name; }
		[[nodiscard]] double GetPriceUnit() const { return m_priceUnit; }
	};
}