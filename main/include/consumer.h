#ifndef CONSUMER_H
#define CONSUMER_H

#include <type_traits>

#include "error.h"
#include "motor.h"
#include "hal.h"

namespace Consumer {
    enum class MessageTag {
        MOTOR_COMMAND
        // TODO: IMU read, send data to ROS server, etc.
    };

    union MessageBody {
        Motor::Command motor_cmd;
    };

    struct Message {
        MessageTag tag;
        MessageBody body;
    };

    Result push_to_queue(MessageTag tag, MessageBody body);
    Result pop_from_queue(MessageTag& tag, MessageBody& body);
    Result push_motor_command(Motor::Command cmd);

    template <
        typename MotorDriver, 
        typename DriveStyle,
        size_t N,
        typename = HAL::MotorDriverTrait<MotorDriver>,
        typename = HAL::DriveStyleTrait<DriveStyle, N>
    >
    void spin(MotorDriver driver) {
        // DriveStyle::convert_twist(std::array<Motor::Command, 2>{});

        while (1) {
            MessageTag tag;
            MessageBody body;

            if (pop_from_queue(tag, body) != Result::SUCCESS) {
                continue;
            }

            switch (tag) {
                case MessageTag::MOTOR_COMMAND:
                    // TODO: handle motor command message
                    break;

                default:
                    fatal("Unhandled message type: tag=%d", tag);
                    break;
            }
        }
    }
}

#endif