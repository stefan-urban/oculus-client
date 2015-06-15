#ifndef EDVSEVENTHANDLER_H
#define EDVSEVENTHANDLER_H

#include "vendor/edvstools/Edvs/Event.hpp"

class EdvsEventHandler
{
public:
    virtual void handle_event(Edvs::Event event) = 0;
};

#endif // EDVSEVENTHANDLER_H
