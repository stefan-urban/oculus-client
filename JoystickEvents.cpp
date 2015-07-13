#include "JoystickEvents.hpp"

#include <cstring>

typedef struct {
    int button;
    int state;
} joystick_event_t;



std::vector<unsigned char> JoystickEvents::serialize()
{
    joystick_event_t e;
    e.button = button_;
    e.state = state_;

    // Copy
    std::vector<unsigned char> data;
    std::memcpy(data.data(), &e, sizeof(joystick_event_t));

    return data;
}

void JoystickEvents::unserialize(std::vector<unsigned char> *data)
{
    joystick_event_t e;
    std::memcpy(&e, data->data(), sizeof(joystick_event_t));

    button_ = e.button;
    state_ = e.state;
}
