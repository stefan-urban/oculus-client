#ifndef EVENT_KEYINPUT_HPP
#define EVENT_KEYINPUT_HPP

#include <vector>

class Event_KeyInput
{
public:
    enum { type_id = 20 };

    Event_KeyInput(int key, int scancode, int action, int mods)
        : key_(key)
        , scancode_(scancode)
        , action_(action)
        , mods_(mods)
    {
    }

    Event_KeyInput()
    {
    }

    std::vector<unsigned char> serialize();
    void unserialize(std::vector<unsigned char> *data);

private:
    int key_;
    int scancode_;
    int action_;
    int mods_;
};

#endif // EVENT_KEYINPUT_HPP
