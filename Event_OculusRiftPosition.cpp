#include "Event_OculusRiftPosition.hpp"

#include <cstring>


typedef struct {
    glm::vec4 orientation;
    glm::vec3 position;
} tracking_info_t;


std::vector<unsigned char> Event_OculusRiftPosition::serialize()
{
    tracking_info_t t;
    t.orientation = orientation_;
    t.position = position_;

    // Copy
    std::vector<unsigned char> data;
    data.resize(sizeof(tracking_info_t));

    std::memcpy(data.data(), &t, sizeof(tracking_info_t));

    return data;
}

void Event_OculusRiftPosition::unserialize(std::vector<unsigned char> const *data)
{
    tracking_info_t t;
    std::memcpy(&t, data->data(), sizeof(tracking_info_t));

    orientation_ = t.orientation;
    position_ = t.position;
}
