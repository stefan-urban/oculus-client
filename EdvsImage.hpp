#ifndef EDVSIMAGE_H
#define EDVSIMAGE_H


#include <boost/chrono.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "EdvsEventHandler.hpp"
#include "vendor/edvstools/Edvs/Event.hpp"
#include "vendor/oculus-server/EdvsEventsCollection.hpp"



class EdvsImage
{
public:
    EdvsImage();
    void handle_event(Edvs::Event event);
    EdvsEventsCollection *events();

private:
    EdvsEventsCollection events_;


    void clear_old_events();

    int decay = 0.5;
};


#endif // EDVSIMAGE_H
