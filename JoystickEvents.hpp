#ifndef JOYSTICKEVENT_H
#define JOYSTICKEVENT_H

#include <vector>

class JoystickEvents
{
public:
    enum { type_id = 21 };

    enum { ButtonPressed = 0};
    enum { ButtonReleased = 0};

    JoystickEvents(int button, int state)
        : button_(button)
        , state_(state)
    {
    }

    JoystickEvents()
    {
    }

    std::vector<unsigned char> serialize();
    void unserialize(std::vector<unsigned char> *data);

private:
    int button_;
    int state_;
};

#endif // JOYSTICKEVENT_H
