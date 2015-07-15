#ifndef EVENT_OCULUSRIFTPOSITION_HPP
#define EVENT_OCULUSRIFTPOSITION_HPP

#include "vendor/dispatcher/Dispatcher.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>



class Event_OculusRiftPosition
{
public:
    enum { type_id = 30 };

    Event_OculusRiftPosition()
    {
    }

    Event_OculusRiftPosition(glm::vec4 orientation, glm::vec3 position)
        : orientation_(orientation)
        , position_(position)
    {
    }

    unsigned char get_type()
    {
        return (unsigned char) type_id;
    }

    std::vector<unsigned char> serialize();
    void unserialize(std::vector<unsigned char> const *data);

private:
    glm::vec4 orientation_;
    glm::vec3 position_;
};

#endif // EVENT_OCULUSRIFTPOSITION_HPP
