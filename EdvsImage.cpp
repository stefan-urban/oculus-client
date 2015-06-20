#include "EdvsImage.hpp"


void EdvsImage::handle_event(Edvs::Event event)
{
    events_.push_back(event);
}

EdvsEventsCollection *EdvsImage::events()
{
    return &events_;
}

void EdvsImage::clear_old_events()
{
    events_.clear();
}
