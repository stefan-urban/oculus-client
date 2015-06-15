#include "EdvsImage.hpp"



EdvsImage::EdvsImage()
{
    image = new boost::numeric::ublas::matrix<weighted_events_list_t>(3,3);
}

void EdvsImage::handle_event(Edvs::Event *event)
{
    weighted_event_t e;
    e.parity = (bool) event->parity;
    e.time = boost::chrono::microseconds(event->t);

    image->at_element(event->x, event->y).push_back(e);
}

boost::numeric::ublas::matrix<weighted_events_list_t> *EdvsImage::get_image()
{
    return image;
}
