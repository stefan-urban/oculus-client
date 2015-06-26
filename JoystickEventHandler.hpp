#ifndef JOYSTICKEVENTHANDLER_HPP
#define JOYSTICKEVENTHANDLER_HPP

#include "Common.h"
#include "vendor/joystick/joystick.hh"

class JoystickEventHandler
{
public:
    JoystickEventHandler()
    {
        joystick_ = new Joystick;
    }

    JoystickEventHandler(Joystick *joystick)
        : joystick_(joystick)
    {
    }

    int run();

private:
    Joystick *joystick_;

    char number_of_buttons_;
    char number_of_axis_;

    std::vector<bool> button_states_;
    std::vector<int> axis_states_;

    void button_pressed(int id);
    void button_released(int id);
    void axis_update(int id, int new_value);
};

#endif // JOYSTICKEVENTHANDLER_HPP
