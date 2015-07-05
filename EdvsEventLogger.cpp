#include "EdvsEventLogger.hpp"

#include <fstream>
#include <iomanip>

EdvsEventLogger::EdvsEventLogger()
{
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

void EdvsEventLogger::event(DispatcherEvent* event)
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

void EdvsEventLogger::update()
{
    std::array<std::ofstream, 7> logfile;

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


    for (size_t i = 0; i < it_; i++)
    {
        logfile.at(events_[i].id) << std::setw(3) << std::to_string(events_[i].x) << " " << std::setw(3) << std::to_string(events_[i].y) << " " << std::to_string(events_[i].parity) << std::endl;
    }

    for (size_t i = 0; i < 7; i++)
    {
        logfile.at(i).close();
    }

//    std::cout << "Did write " << std::to_string(it_) << " events to files (time diff: " << std::to_string(events_[0].t - events_[it_].t) << ")" << std::endl;

    // "Clear" array
    it_ = 0;
}

std::string EdvsEventLogger::create_path(std::string file, int i)
{
    return file + "_" + std::to_string(i) + ".dvs";
}
