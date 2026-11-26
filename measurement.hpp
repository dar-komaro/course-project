#pragma once
#include <string>
#include <cstdlib>

namespace greenhouse {

    class IMeasurementStrategy {
    public:
        virtual ~IMeasurementStrategy() = default;

        // расход батареи
        [[nodiscard]] virtual int GetBatteryDrain() const = 0;

        // новое показание с учетом целевого значения и разброса режима
        [[nodiscard]] virtual double ComputeValue(double targetValue) const = 0;

        // Название
        [[nodiscard]] virtual std::string GetModeName() const = 0;
    };

    // Обычный режим (высокая точность, обычный расход)
    class StandardMode : public IMeasurementStrategy {
    public:
        [[nodiscard]] int GetBatteryDrain() const override {
            return 5;
        }

        [[nodiscard]] double ComputeValue(double targetValue) const override {
            // Небольшой разброс: +/- 1.0
            return targetValue + (std::rand() % 5 - 2) * 0.5;
        }

        [[nodiscard]] std::string GetModeName() const override {
            return "Стандартный";
        }
    };

    // Энергосберегающий режим (экономит батарею, больше погрешность)
    class EcoMode : public IMeasurementStrategy {
    public:
        [[nodiscard]] int GetBatteryDrain() const override {
            return 1;
        }

        [[nodiscard]] double ComputeValue(double targetValue) const override {
            // Больший разброс: +/- 4.0
            return targetValue + (std::rand() % 9 - 4) * 1.0;
        }

        [[nodiscard]] std::string GetModeName() const override {
            return "Эко (энергосбережение)";
        }
    };

} // namespace greenhouse
