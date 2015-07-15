#ifndef EVENT_EDVSEVENTSUPDATE_HPP
#define EVENT_EDVSEVENTSUPDATE_HPP

#include <GL/gl.h>
#include <vector>

class Event_EdvsEventsUpdate
{
public:
    enum { type_id = 40 };

    Event_EdvsEventsUpdate()
    {
    }

    Event_EdvsEventsUpdate(std::vector<GLfloat> parity, std::vector<GLfloat> position, std::vector<GLfloat> camera_id, std::vector<unsigned long long> time)
        : parity_(parity)
        , position_(position)
        , camera_id_(camera_id)
        , time_(time)
    {
    }

    std::vector<unsigned char> serialize();
    void unserialize(std::vector<unsigned char> const *data);


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

};

#endif // EVENT_EDVSEVENTSUPDATE_HPP
