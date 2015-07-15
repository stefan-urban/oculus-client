#include "App_EdvsEventLogger.hpp"

#include <fstream>
#include <iomanip>
#include <chrono>

App_EdvsEventLogger::App_EdvsEventLogger()
{
    std::chrono::steady_clock::time_point start_ = std::chrono::steady_clock::now();

    std::array<std::ofstream, 7> logfile;

    for (size_t i = 0; i < 7; i++)
    {
        std::string path = create_path(logfile_, i);
        logfile.at(i).open(path, std::ofstream::out | std::ofstream::trunc);

        if (logfile.at(i).is_open())
        {
            logfile.at(i).close();
        }
    }
}

void App_EdvsEventLogger::event(DispatcherEvent* event)
{
    Message_EventCollection2 msg_events;
    msg_events.unserialize(event->data());

    for(message_edvs_event_t& e : msg_events.events())
    {
        events_[it_++] = e;

        if (it_ >= max_events)
        {
            update();
        }
    }
}

void App_EdvsEventLogger::update()
{
    std::array<std::ofstream, 7> logfile;

    // Open up all logfiles
    for (size_t i = 0; i < 7; i++)
    {
        std::string path = create_path(logfile_, i);
        logfile.at(i).open(path, std::ofstream::out | std::ios::app);

        if (!logfile.at(i).is_open())
        {
            std::cout << "could not open logfile: " << path << std::endl;
            return;
        }
    }

    auto duration = std::chrono::steady_clock::now() - start_;
    unsigned long long time = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

    // Write events
    for (size_t i = 0; i < it_; i++)
    {
        logfile.at(events_[i].id) << std::setw(3) << std::to_string(events_[i].x) << " " << std::setw(3) << std::to_string(events_[i].y) << " " << std::to_string(events_[i].parity) << " " << std::to_string(time) << std::endl;
    }

    // Close logfiles
    for (size_t i = 0; i < 7; i++)
    {
        logfile.at(i).close();
    }

    // "Clear" array
    it_ = 0;
}

std::string App_EdvsEventLogger::create_path(std::string file, int i)
{
    return file + "_" + std::to_string(i) + ".dvs";
}
