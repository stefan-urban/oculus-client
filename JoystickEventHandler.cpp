#include "JoystickEventHandler.hpp"

extern int global_stop;

int JoystickEventHandler::run()
{
    if (!joystick_->isFound())
    {
        Platform::sleepMillis(500);

        joystick_->reconnect();
        run();
    }

    std::cout << "Joystick found" << std::endl;

    // Grab basic information
    number_of_buttons_ = joystick_->numberOfButtons();
    button_states_.resize(number_of_buttons_);

    number_of_axis_ = joystick_->numberOfAxis();
    axis_states_.resize(number_of_axis_);

    while (global_stop == 0)
    {
        // Always check if joystick is still connected
        if (!joystick_->isFound())
        {
            std::cout << "Joystick detached" << std::endl;

            // Reset states
            std::fill(button_states_.begin(), button_states_.end(), false);
            std::fill(axis_states_.begin(), axis_states_.end(), 0);

            // Restart
            run();
        }

        // Attempt to sample an event from the joystick
        JoystickEvent event;

        if (joystick_->sample(&event))
        {
            if (event.isButton())
            {
                if (event.value == 0)
                {
                    button_released((int) event.number);
                }
                else
                {
                    button_pressed((int) event.number);
                }
            }
            else if (event.isAxis())
            {
                axis_update((int) event.number, (int) event.value);
            }
        }

        // Transmit changes
        //...

        Platform::sleepMillis(50);
    }

    return 0;
}

void JoystickEventHandler::button_pressed(int id)
{
    button_states_[id] = true;
}

void JoystickEventHandler::button_released(int id)
{
    button_states_[id] = false;
}

void JoystickEventHandler::axis_update(int id, int new_value)
{
    axis_states_[id] = new_value;
}
