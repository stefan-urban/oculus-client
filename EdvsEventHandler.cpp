#include "EdvsEventHandler.hpp"
#include "vendor/oculus-server/Message_EventCollection.hpp"


void EdvsEventHandler::event(DispatcherEvent* event)
{
    Message_EventCollection msg_events;

    std::string data = event->data();
    msg_events.unserialize(&data);

    mutex_->lock();

    for(Edvs::Event& e : msg_events.events())
    {
        if (camera_id_.size() >= max_events_number_)
        {
            std::cout << "max events size reached, allocate more memory" << std::endl;
            continue;
        }

        if (e.id < 7)
        {
            camera_id_.push_back((GLuint) e.id);
            position_.push_back(((GLfloat) (e.y)) / 128.0);
            position_.push_back(((GLfloat) (e.x)) / 128.0);
            parity_.push_back((GLfloat) (e.parity ? 1.0f : -1.0f));
            time_.push_back(e.t);
        }
    }

    mutex_->unlock();
}

void EdvsEventHandler::clear()
{
    mutex_->lock();

    camera_id_.clear();
    position_.clear();
    parity_.clear();
    time_.clear();

    mutex_->unlock();
}

void EdvsEventHandler::update()
{
    long long unsigned max_time = 100 * 1000; // 100 ms
}
