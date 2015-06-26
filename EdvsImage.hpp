#ifndef EDVSIMAGE_H
#define EDVSIMAGE_H


#include <chrono>
#include "vendor/edvstools/Edvs/Event.hpp"



class EdvsImage
{
public:
    EdvsImage();
    void add_event(Edvs::Event *event);

    void update();

    float data(int i)
    {
        return image_[i];
    }

    size_t size()
    {
        return 128*128;
    }

private:
    float image_[128*128];

    // Decay in a microsecond timescale
    float decay_ = 100 * 1000;

    //
    std::chrono::high_resolution_clock::time_point t_lastupdate;

};


#endif // EDVSIMAGE_H
