#ifndef JOYSTICKEVENTHANDLER_HPP
#define JOYSTICKEVENTHANDLER_HPP

#include "Common.h"
#include "vendor/oculus-server/TcpMessage.hpp"
#include "vendor/oculus-server/Message_JoystickState.hpp"
#include "vendor/joystick/joystick.hh"

class JoystickEventHandler
{
public:
    JoystickEventHandler()
    {
        joystick_ = new Joystick;
        setup_state_vector_sizes();
    }

    JoystickEventHandler(Joystick *joystick)
        : joystick_(joystick)
    {
        setup_state_vector_sizes();
    }

    int handle_events();

    std::vector<bool> button_states()
    {
        return button_states_;
    }

    bool button(std::size_t id)
    {
        return button_states_.at(id);
    }

    std::vector<int> axis_states()
    {
        return axis_states_;
    }

    int axis(std::size_t id)
    {
        return axis_states_.at(id);
    }


private:
    Joystick *joystick_;

    char number_of_buttons_;
    char number_of_axis_;

    std::vector<bool> button_states_;
    std::vector<int> axis_states_;
    void setup_state_vector_sizes();

    void button_pressed(int id);
    void button_released(int id);
    void axis_update(int id, int new_value);
};

#endif // JOYSTICKEVENTHANDLER_HPP
