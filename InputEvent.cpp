#include "InputEvent.hpp"

#include <string>
#include <sstream>
#include <vector>

std::vector<unsigned char> InputEvent::serialize()
{
    std::string str;

    str.append(std::to_string(key_));
    str.append("|");

    str.append(std::to_string(scancode_));
    str.append("|");

    str.append(std::to_string(action_));
    str.append("|");

    str.append(std::to_string(mods_));
    str.append("|");

    return std::vector<unsigned char>(str.begin(), str.end());
}

void InputEvent::unserialize(std::vector<unsigned char> *data)
{
    auto str = std::string(data->begin(), data->end());

    // Key
    std::size_t pos = str.find("|");

    if (pos == std::string::npos)
    {
        return;
    }

    std::string str_buf = str.substr(0, pos);
    std::istringstream(str_buf) >> key_;

    str = str.substr(pos + 1);

    // Scancode
    pos = str.find("|");

    if (pos == std::string::npos)
    {
        return;
    }

    str_buf = str.substr(0, pos);
    std::istringstream(str_buf) >> scancode_;

    str = str.substr(pos + 1);

    // Action
    pos = str.find("|");

    if (pos == std::string::npos)
    {
        return;
    }

    str_buf = str.substr(0, pos);
    std::istringstream(str_buf) >> action_;

    str = str.substr(pos + 1);

    // Mods
    std::istringstream(str) >> mods_;
}
