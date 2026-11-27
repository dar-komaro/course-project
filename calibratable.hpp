#pragma once
#include <string>

namespace greenhouse {

    class ICalibratable {
    public:
        virtual ~ICalibratable() = default;

        virtual void Calibrate(double offset) = 0;

        [[nodiscard]] virtual bool IsCalibrated() const = 0;
    };

} // namespace greenhouse
