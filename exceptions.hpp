#pragma once
#include <stdexcept>
#include <string>
#include <string_view>

namespace greenhouse {

    // Базовое исключение теплицы (наследуем от std::runtime_error)
    class GreenhouseException : public std::runtime_error {
    public:
        explicit GreenhouseException(const std::string& message)
            : std::runtime_error(message) {}
    };

    // Исключение 1: Недостаточно заряда для замера
    class LowBatteryException : public GreenhouseException {
    public:
        explicit LowBatteryException(std::string_view sensorName, int currentBattery)
            : GreenhouseException(
                "Недостаточно заряда на датчике \"" + std::string(sensorName) +
                "\" (осталось " + std::to_string(currentBattery) + "%)"
            ) {
        }
    };

    // Исключение 2: Некорректная поправка при калибровке
    class InvalidCalibrationException : public GreenhouseException {
    public:
        explicit InvalidCalibrationException(std::string_view sensorName, double offset)
            : GreenhouseException(
                "Недопустимое смещение калибровки (" + std::to_string(offset) +
                ") для датчика \"" + std::string(sensorName) + "\""
            ) {
        }
    };

} // namespace greenhouse
