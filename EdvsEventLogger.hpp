#ifndef EDVSEVENTLOGGER_HPP
#define EDVSEVENTLOGGER_HPP

#include <array>
#include <string>
#include <iostream>
#include <chrono>

#include "vendor/dispatcher/Dispatcher.hpp"
#include "vendor/oculus-server/Message_EventCollection2.hpp"

class EdvsEventLogger : public DispatcherListener
{
public:
    EdvsEventLogger();
    void event(DispatcherEvent* event);
    void update();

private:
    enum { max_events = 10000 };

    std::string logfile_ = "logfiles/data";
    std::string create_path(std::string file, int i);

    std::array<message_edvs_event_t, max_events> events_;
    size_t it_ = 0;

    std::chrono::steady_clock::time_point start_;
};

#endif // EDVSEVENTLOGGER_HPP
