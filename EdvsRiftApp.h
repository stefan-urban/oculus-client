#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsImage.hpp"

#include <chrono>

class EdvsRiftApp : public RiftApp
{
public:
    EdvsRiftApp(std::vector<EdvsImage> *images)
        : images_(*images)
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

    std::vector<EdvsImage> images_;

    void drawSphere();
    void drawEdvsEvent(glm::mat4 transform, float intensity);

    std::shared_ptr<oglplus::VertexArray> vao;
    std::shared_ptr<oglplus::Buffer> vab;

    std::chrono::system_clock::time_point t_start;
};


#endif // EDVSRIFTAPP_H
