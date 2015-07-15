#ifndef APP_EDVSEVENTHANDLER_H
#define APP_EDVSEVENTHANDLER_H

#include <vector>
#include <boost/thread.hpp>

#include "Common.h"
#include "vendor/dispatcher/Dispatcher.hpp"


class App_EdvsEventHandler : public DispatcherListener
{
public:
    enum { max_event_time = 50 * 1000 };

    App_EdvsEventHandler(boost::mutex *mutex, Dispatcher *dispatcher)
        : mutex_(mutex)
        , dispatcher_(dispatcher)
    {
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
    Dispatcher *dispatcher_;


    const int max_events_number_ = 50000;
};

#endif // APP_EDVSEVENTHANDLER_H
