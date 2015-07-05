#include "EdvsEventHandler.hpp"
#include "vendor/oculus-server/Message_EventCollection2.hpp"

#include <cmath>
#include <random>

void EdvsEventHandler::event(DispatcherEvent* event)
{
    Message_EventCollection2 msg_events;
    msg_events.unserialize(event->data());

    for(message_edvs_event_t& e : msg_events.events())
    {
        if (camera_id_.size() >= max_events_number_)
        {
            std::cout << "max events size reached, allocate more memory" << std::endl;
            continue;
        }

        //std::cout << "time: " << e.t << std::endl;

        if (e.id < 7)
        {
            mutex_->lock();

            camera_id_.push_back((GLuint) e.id);
            position_.push_back(((GLfloat) (e.x)) / 128.0);
            position_.push_back(((GLfloat) (e.y)) / 128.0);
            parity_.push_back((GLfloat) (e.parity ? 1.0f : -1.0f));
//            time_.push_back(e.t);
            time_.push_back(0);

            mutex_->unlock();
        }
    }
}

void EdvsEventHandler::clear()
{
    camera_id_.clear();
    position_.clear();
    parity_.clear();
    time_.clear();
}

void EdvsEventHandler::update()
{
    mutex_->lock();

    for (size_t i = 0; i < camera_id_.size(); i++)
    {
        if (std::fabs(parity_[i]) < 0.9)
        {
            camera_id_.erase(camera_id_.begin() + i);
            position_.erase(position_.begin() + 2*i);
            position_.erase(position_.begin() + 2*i);
            parity_.erase(parity_.begin() + i);
            time_.erase(time_.begin() + i);
        }
        else
        {
            parity_[i] *= 0.5;
        }
    }

    mutex_->unlock();
}
