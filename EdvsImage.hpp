#ifndef EDVSIMAGE_H
#define EDVSIMAGE_H


#include <boost/chrono.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "EdvsEventHandler.hpp"
#include "vendor/edvstools/Edvs/Event.hpp"


typedef struct {
    bool parity;
    boost::chrono::microseconds time;
    double weight;
} weighted_event_t;

typedef std::vector<weighted_event_t> weighted_events_list_t;


class EdvsImage
{
public:
    EdvsImage();
    void handle_event(Edvs::Event *event);
    boost::numeric::ublas::matrix<weighted_events_list_t> *get_image();

private:
    boost::numeric::ublas::matrix<weighted_events_list_t>* image;
    double decay = 0.5;
};


#endif // EDVSIMAGE_H
