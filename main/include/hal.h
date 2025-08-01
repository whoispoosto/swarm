#ifndef HAL_H
#define HAL_H

#include <array>
#include <type_traits>

#include "motor.h"
#include "drive.h"

/*
The design philosophy implemented here is as follows:
- Define abstract types: Motor, etc.
- Use the abstract types to define HAL types
- Use the HAL-defined traits and types to implement a specific driver (e.g., ESP32 Motor Driver)
- Create a global instance (or instances) of the specific driver in the hardware namespace
- Use the global instances throughout application logic (e.g., Consumer task)
*/

/*
HAL Interface
The traits and enums defined in this namespace can be used to create drivers for any board, i.e., ESP32.
*/
namespace HAL {
    enum class Voltage {
        HIGH = 1,
        LOW = 0
    };

    template <bool B, typename T = void>
    using Trait = std::enable_if_t<B, T>;

    template <typename T>
    using MotorDriverTrait = Trait<std::conjunction_v<
        std::is_invocable_r<void, decltype(&T::run), T&, Motor::Command>,
        std::is_invocable_r<void, decltype(&T::stop), T&>
    >>;

    template <typename T, size_t N>
    using DriveStyleTrait = Trait<std::conjunction_v<
        std::negation<std::is_constructible<T>>,
        std::is_invocable_r<Drive::Type, decltype(&T::type)>,
        // TODO: actually take twist message
        std::is_invocable_r<void, decltype(&T::template convert_twist<N>), std::array<Motor::Command, N>>
    >>;
}

#endif