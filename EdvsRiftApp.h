#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsImage.hpp"


class EdvsRiftApp : public RiftApp
{
public:
    EdvsRiftApp(std::vector<EdvsImage> *images)
        : images_(*images)
    {

    }

    void drawSphere();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

private:
    float projection_scale = 1.0f;
    float trans = 0.0f;
    float fontsize = 20.0f;

    std::vector<EdvsImage> images_;
};


#endif // EDVSRIFTAPP_H


