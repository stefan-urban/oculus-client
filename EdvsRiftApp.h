#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsImage.hpp"

#include <chrono>

class EdvsRiftApp : public RiftApp
{
public:
    EdvsRiftApp(std::vector<Edvs::Event> *events)
        : events_(events)
    {

    }

    void initGl();
    void update();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

private:
    float projection_scale = 1.0f;
    float trans = 0.0f;
    float fontsize = 20.0f;

    std::vector<Edvs::Event> *events_;

    void drawSphere();
};


#endif // EDVSRIFTAPP_H
