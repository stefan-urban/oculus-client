#include "EdvsImage.hpp"


EdvsImage::EdvsImage()
{
    for (int i = 0; i < 128; i += 2)
    {
        for (int j = 0; j < 128; j += 2)
        {
            Edvs::Event event;

            event.x = i;
            event.y = j;
            event.parity = 0;
            event.t = 0;

            events_.push_back(event);
        }
    }
}

void EdvsImage::handle_event(Edvs::Event event)
{
    //events_.push_back(event);
}

EdvsEventsCollection *EdvsImage::events()
{
    return &events_;
}

void EdvsImage::clear_old_events()
{
    events_.clear();
}
