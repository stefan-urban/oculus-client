#include "EdvsImageHandler.hpp"
#include "vendor/oculus-server/Message_EventCollection.hpp"

#include <string>

void EdvsImageHandler::event(DispatcherEvent event)
{
    if (event.type().compare("events") != 0)
    {
        // Wrong event type
        return;
    }

    Message_EventCollection msg_events;
    std::string data = event.data();

    msg_events.unserialize(&data);

    for(Edvs::Event& e : msg_events.events())
    {
        if (e.id < 7)
        {
            images_[e.id].add_event(&e);
        }
    }
}
