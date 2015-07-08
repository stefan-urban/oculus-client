#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <vector>

class InputEvent
{
public:
    enum { type_id = 20 };

    InputEvent(int key, int scancode, int action, int mods)
        : key_(key)
        , scancode_(scancode)
        , action_(action)
        , mods_(mods)
    {
    }

    InputEvent()
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

#endif // INPUTEVENT_H
