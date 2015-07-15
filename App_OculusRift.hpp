#ifndef APP_OCULUSRIFT_HPP
#define APP_OCULUSRIFT_HPP

#include "Common.h"
#include "vendor/dispatcher/Dispatcher.hpp"

#include <chrono>
#include <boost/thread.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/opt/resources.hpp>


typedef struct {
    glm::vec4 orientation;
    glm::vec3 position;
} tracking_info_t;



class App_OculusRift : public RiftApp, public DispatcherListener
{
public:
    enum { type_id = 13 };

    App_OculusRift(boost::mutex *mutex, Dispatcher *dispatcher)
        : mutex_(mutex)
        , dispatcher_(dispatcher)
    {
    }

    void initGl();
    void update();
    void renderScene();
    void onKey(int key, int scancode, int action, int mods);

    void event(DispatcherEvent* event);

private:
    float azimuth = 0.0;
    float elevation = -20.0 * DEGREES_TO_RADIANS;

    void drawEvents();
    void drawSphereBackground(int camera);

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


#endif // APP_OCULUSRIFT_HPP
