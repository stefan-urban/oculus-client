#ifndef EDVSEVENTLOGGER_HPP
#define EDVSEVENTLOGGER_HPP

#include <array>
#include <string>

#include "vendor/edvstools/Edvs/Event.hpp"
#include "vendor/dispatcher/Dispatcher.hpp"

class EdvsEventLogger : public DispatcherListener
{
public:
    EdvsEventLogger();
    void event(DispatcherEvent* event);
    void update();

private:
    enum { max_events = 10000 };

    std::string logfile_ = "logfiles/test";
    std::string create_path(std::string file, int i);

    std::array<Edvs::Event, max_events> events_;
    size_t it_ = 0;
};

#endif // EDVSEVENTLOGGER_HPP
