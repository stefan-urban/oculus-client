#include "JoystickEventHandler.hpp"


int JoystickEventHandler::handle_events()
{
    // Attempt to sample an event from the joystick
    std::vector<JoystickEvent> events;
    joystick_->samples(&events);

    for(JoystickEvent event : events)
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

    return 1;
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

void JoystickEventHandler::setup_state_vector_sizes()
{
    number_of_buttons_ = joystick_->numberOfButtons();
    button_states_.resize(number_of_buttons_);

    number_of_axis_ = joystick_->numberOfAxis();
    axis_states_.resize(number_of_axis_);
}
