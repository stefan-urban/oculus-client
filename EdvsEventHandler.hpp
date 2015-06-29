#ifndef EDVSEVENTHANDLER_H
#define EDVSEVENTHANDLER_H

#include <vector>
#include <boost/thread.hpp>

#include "Common.h"
#include "vendor/dispatcher/Dispatcher.hpp"


class EdvsEventHandler : public DispatcherListener
{
public:
    EdvsEventHandler(boost::mutex *mutex)
        : mutex_(mutex)
    {
        /*
        camera_id_.push_back((GLfloat) 0);
        position_.push_back(((GLfloat) (128 - 64)) / 128.0);
        position_.push_back(((GLfloat) (128 - 64)) / 128.0);
        parity_.push_back((GLfloat) (1.0f));
        time_.push_back(0);

        camera_id_.push_back((GLfloat) 1);
        position_.push_back(((GLfloat) (128 - 62)) / 128.0);
        position_.push_back(((GLfloat) (128 - 62)) / 128.0);
        parity_.push_back((GLfloat) (-1.0f));
        time_.push_back(0);
        */

        parity_.reserve(max_events_number_);
        position_.reserve(max_events_number_ * 2);
        camera_id_.reserve(max_events_number_);
        time_.reserve(max_events_number_);
    }

    void event(DispatcherEvent* event);
    void clear();
    void update();

    std::vector<GLfloat> parity()
    {
        return parity_;
    }

    std::vector<GLfloat> position()
    {
        return position_;
    }

    std::vector<GLfloat> camera_id()
    {
        return camera_id_;
    }

    std::vector<unsigned long long> time()
    {
        return time_;
    }


private:
    std::vector<GLfloat> parity_;
    std::vector<GLfloat> position_;
    std::vector<GLfloat> camera_id_;
    std::vector<unsigned long long> time_;

    boost::mutex *mutex_;

    const int max_events_number_ = 50000;
};

#endif // EDVSEVENTHANDLER_H
