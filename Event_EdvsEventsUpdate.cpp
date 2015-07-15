#include "Event_EdvsEventsUpdate.hpp"

#include <array>
#include <cstring>

typedef struct __attribute__((packed)) {
    unsigned int x      : 8;
    unsigned int y      : 8;
    unsigned int parity : 1;
    unsigned int id     : 7;
} evds_camera_event_t;

std::vector<unsigned char> Event_EdvsEventsUpdate::serialize()
{
    std::vector<evds_camera_event_t> events;

    for (size_t i = 0; i < camera_id_.size(); i++)
    {
        events.at(i).x = position_.at(2 * i);
        events.at(i).y = position_.at(2 * i + 1);
        events.at(i).parity = parity_.at(i);
        events.at(i).id = camera_id_.at(i);
    }

    // Copy
    std::vector<unsigned char> data;
    data.resize(sizeof(evds_camera_event_t) * events.size());

    std::memcpy(data.data(), events.data(), sizeof(evds_camera_event_t) * events.size());

    return data;
}

void Event_EdvsEventsUpdate::unserialize(std::vector<unsigned char> const *data)
{
    std::vector<evds_camera_event_t> events;
    events.resize(data->size());

    std::memcpy(events.data(), data->data(), sizeof(evds_camera_event_t) * events.size());

    for (size_t i = 0; i < events.size(); i++)
    {
        position_.push_back(events.at(i).x);
        position_.push_back(events.at(i).y);

        parity_.push_back(events.at(i).parity);
        camera_id_.push_back(events.at(i).id);

    }
}
