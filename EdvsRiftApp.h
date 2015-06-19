#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsSphericImage.hpp"


class EdvsRiftApp : public RiftApp
{
public:
    EdvsRiftApp()
    {

    }

    void drawSphere();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

private:
    float projection_scale = 1.0f;
    float trans = 0.0f;
};


#endif // EDVSRIFTAPP_H


