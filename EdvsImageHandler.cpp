#include "EdvsImageHandler.hpp"
#include "vendor/oculus-server/Message_EventCollection.hpp"

#include <chrono>
#include <string>
#include <utility>

void EdvsImageHandler::event(DispatcherEvent event)
{
    Message_EventCollection msg_events;

    std::string data = event.data();
    msg_events.unserialize(&data);

    for(Edvs::Event& e : msg_events.events())
    {
        if (e.id < 7)
        {
            float parity = e.parity ? 1.0f : -1.0f;
            auto position = std::pair<int, int>(e.x, e.y);
            unsigned long long time = e.t;

            cameras_[e.id].add(parity, position, time);
        }
    }
}
