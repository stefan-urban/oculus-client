#include "Common.h"
#include "EdvsRiftApp.h"

#include <random>
#include <cstring>
#include <glm/gtx/rotate_vector.hpp>

using namespace oglplus;


void EdvsRiftApp::initGl()
{
    RiftApp::initGl();

    vao = VertexArrayPtr(new VertexArray());
    vbo_position = BufferPtr(new Buffer());
    vbo_color = BufferPtr(new Buffer());
    vbo_camera_id = BufferPtr(new Buffer());
}

void EdvsRiftApp::update()
{
    RiftApp::update();

    mutex_->lock();
    parity_ = edvs_event_handler_->parity();
    position_ = edvs_event_handler_->position();
    camera_id_ = edvs_event_handler_->camera_id();
    time_ = edvs_event_handler_->time();
    mutex_->unlock();

    ovrTrackingState tracking_state = ovrHmd_GetTrackingState(hmd, 0.0);

    auto orientation = glm::vec4(
        tracking_state.CameraPose.Orientation.x,
        tracking_state.CameraPose.Orientation.y,
        tracking_state.CameraPose.Orientation.z,
        tracking_state.CameraPose.Orientation.w
        );

    auto position = glm::vec3(
        tracking_state.CameraPose.Position.x,
        tracking_state.CameraPose.Position.y,
        tracking_state.CameraPose.Position.z
        );

    tracking_info_t info = {
        orientation,
        position
    };

    std::vector<unsigned char> data;
    data.resize(sizeof(tracking_info_t));

    std::memcpy(&data[0], &info, sizeof(tracking_info_t));

    auto e = DispatcherEvent(type_id, &data);
    dispatcher_->dispatch(&e);
}

void EdvsRiftApp::drawEvents()
{
    static ProgramPtr program = oria::loadProgram("./resources/event_pixel.vs", "./resources/event_pixel.fs");

    // Reset before application exit
    Platform::addShutdownHook([]
    {
        program.reset();
    });


    MatrixStack & mv = Stacks::modelview();

    mv.withPush([&]
    {
        // Binds the program
        program->Use();

        // Matrices as uniforms
        Mat4Uniform(*program, "ModelView").Set(Stacks::modelview().top());
        Mat4Uniform(*program, "Projection").Set(Stacks::projection().top());

        // Manual set for azimuth and elevation
        oglplus::Uniform<float>(*program, "ManAzimuth").Set(azimuth);
        oglplus::Uniform<float>(*program, "ManElevation").Set(elevation);

        // Manual set for field of view
        oglplus::Uniform<float>(*program, "FovX").Set(60.0 * DEGREES_TO_RADIANS);
        oglplus::Uniform<float>(*program, "FovY").Set(60.0 * DEGREES_TO_RADIANS);


        // Bind VAO
        vao->Bind();

        // Bind VBO "Positions"
        vbo_position->Bind(Buffer::Target::Array);
        {
            Buffer::Data(Buffer::Target::Array, position_);

            VertexArrayAttrib vao_attr(*program, "Position");
            vao_attr.Setup<Vec2f>();
            vao_attr.Enable();
        }

        // Bind VBO "CameraId"
        vbo_camera_id->Bind(Buffer::Target::Array);
        {
            Buffer::Data(Buffer::Target::Array, camera_id_);

            VertexArrayAttrib vao_attr(*program, "CameraId");
            vao_attr.Setup<Vec1f>();
            vao_attr.Enable();
        }

        // Bind VBO "Color"
        vbo_color->Bind(Buffer::Target::Array);
        {
            Buffer::Data(Buffer::Target::Array, parity_); //, BufferUsage::StreamDraw);

            VertexArrayAttrib vao_attr(*program, "Color");
            vao_attr.Setup<Vec1f>();
            vao_attr.Enable();
        }

        // Draw all elements
        Context::DrawArrays(PrimitiveType::Points, 0, camera_id_.size());
    });

    // Unbind
    oglplus::NoProgram().Bind();
    oglplus::NoVertexArray().Bind();
}

void EdvsRiftApp::drawSphereBackground(int camera_id)
{
    static ProgramPtr program = oria::loadProgram("./resources/sphere_background.vs", "./resources/sphere_background.fs");
    static ShapeWrapperPtr geometry = ShapeWrapperPtr(new shapes::ShapeWrapper({ "Position" }, shapes::ObjMesh(mesh_input.stream), *program));

    // Reset before application exit
    Platform::addShutdownHook([]
    {
        program.reset();
        geometry.reset();
    });


    MatrixStack & mv = Stacks::modelview();

    mv.withPush([&]
    {
        // Binds the program
        program->Use();

        // Matrices as uniforms
        Mat4Uniform(*program, "ModelView").Set(Stacks::modelview().top());
        Mat4Uniform(*program, "Projection").Set(Stacks::projection().top());

        // Draw this
        geometry->Use();
        geometry->Draw();
    });

    // Unbind
    oglplus::NoProgram().Bind();
    oglplus::NoVertexArray().Bind();
}

void EdvsRiftApp::renderScene()
{
    using namespace oglplus;

    Context::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    Context::ClearDepth(1.0f);
    Context::Clear().ColorBuffer();
    Context::Clear().DepthBuffer();


    MatrixStack & mv = Stacks::modelview();

    Context::Disable(Capability::CullFace);
    glDisable(GL_CULL_FACE);


    mv.withPush([&]
    {
        mv.rotate(90 * DEGREES_TO_RADIANS, glm::vec3(1.0, 0.0, 0.0));
        mv.scale(10.1f);

        if (camera_id_.size() > 0)
        {
            drawEvents();
        }
    });

    Context::Enable(Capability::CullFace);
}

void EdvsRiftApp::onKey(int key, int scancode, int action, int mods)
{
    if (GLFW_PRESS == action)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            elevation -= 10 * DEGREES_TO_RADIANS;
            elevation = elevation < -90 * DEGREES_TO_RADIANS ? -90 : elevation;
            return;
        case GLFW_KEY_DOWN:
            elevation += 10 * DEGREES_TO_RADIANS;
            elevation = elevation > 90 * DEGREES_TO_RADIANS ? 90 : elevation;
            return;
        case GLFW_KEY_LEFT:
            azimuth -= 10 * DEGREES_TO_RADIANS;
            return;
        case GLFW_KEY_RIGHT:
            azimuth += 10 * DEGREES_TO_RADIANS;
            return;
        }
    }

    RiftApp::onKey(key, scancode, action, mods);
}
