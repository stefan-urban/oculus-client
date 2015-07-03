#ifndef EDVSRIFTAPP_H
#define EDVSRIFTAPP_H

#include "Common.h"
#include "EdvsEventHandler.hpp"
#include "vendor/dispatcher/Dispatcher.hpp"

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

typedef struct {
    glm::vec4 orientation;
    glm::vec3 position;
} tracking_info_t;



class EdvsRiftApp : public RiftApp
{
public:
    enum { type_id = 13 };

    EdvsRiftApp(EdvsEventHandler *edvs_event_handler, boost::mutex *mutex, Dispatcher *dispatcher)
        : edvs_event_handler_(edvs_event_handler)
        , mutex_(mutex)
        , dispatcher_(dispatcher)
    {
    }

    void initGl();
    void update();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

private:
    float azimuth = 0.0;
    float elevation = -20.0 * DEGREES_TO_RADIANS;

    void drawEvents();
    void drawSphereBackground(int camera);

    MeshInputFile mesh_input;

    EdvsEventHandler *edvs_event_handler_;

    VertexArrayPtr vao;
    BufferPtr vbo_position;
    BufferPtr vbo_color;
    BufferPtr vbo_camera_id;

    boost::mutex *mutex_;
    Dispatcher *dispatcher_;

    std::vector<GLfloat> parity_;
    std::vector<GLfloat> position_;
    std::vector<GLfloat> camera_id_;
    std::vector<unsigned long long> time_;
};


#endif // EDVSRIFTAPP_H
