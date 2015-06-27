#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsImageHandler.hpp"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/opt/resources.hpp>


struct MeshInputFile
{
    std::ifstream stream;

    MeshInputFile(void)
    {
        oglplus::OpenResourceFile(stream, "resources", "spherical_calotte", ".obj");
    }
};


class EdvsRiftApp : public RiftApp
{
public:
    EdvsRiftApp(EdvsImageHandler *image_handler)
    {
        for (size_t i = 0; i < 7; i++)
        {
            images_[i] = &(image_handler->images()[i]);
        }
    }

    void initGl();
    void update();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

private:
    float azimuth = 0.0;
    float elevation = 0.0;

    EdvsImage* images_[7];

    void drawSphere(int camera);

    MeshInputFile mesh_input;
    BufferPtr vbo;
};


#endif // EDVSRIFTAPP_H
