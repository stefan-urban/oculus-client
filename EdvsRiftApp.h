#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsImageHandler.hpp"

#include <chrono>
#include <boost/thread.hpp>
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
    EdvsRiftApp(EdvsImageHandler *image_handler, boost::mutex *mutex)
        : image_handler_(image_handler)
    {
    }

    void initGl();
    void update();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

private:
    float azimuth = 0.0;
    float elevation = 0.0;

    void drawEvents(int camera);
    void drawSphereBackground(int camera);

    MeshInputFile mesh_input;

    EdvsImageHandler *image_handler_;

    VertexArrayPtr vao;
    BufferPtr vbo_position;
    BufferPtr vbo_color;
    BufferPtr indices;

    boost::mutex *mutex_;
};


#endif // EDVSRIFTAPP_H
