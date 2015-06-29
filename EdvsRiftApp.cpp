#include "Common.h"
#include "EdvsRiftApp.h"

#include <random>
#include <glm/gtx/rotate_vector.hpp>

using namespace oglplus;


static EdvsCamera camera[7];

void EdvsRiftApp::initGl()
{
    RiftApp::initGl();

    vao = VertexArrayPtr(new VertexArray());
    vbo_position = BufferPtr(new Buffer());
    vbo_color = BufferPtr(new Buffer());
    indices = BufferPtr(new Buffer());
}

void EdvsRiftApp::update()
{
    RiftApp::update();

    //mutex_->lock();
    camera[0] = *(image_handler_->camera(0));
    //mutex_->unlock();
}

void EdvsRiftApp::drawEvents(int camera_id)
{
    static ProgramPtr program = oria::loadProgram("./resources/event_pixel.vs", "./resources/event_pixel.fs");
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


        // Bind VAO
        vao->Bind();

        // Bind VBO "Positions"
        vbo_position->Bind(Buffer::Target::Array);
        {
            Buffer::Data(Buffer::Target::Array, camera[camera_id].positions(), BufferUsage::StreamDraw);

            VertexArrayAttrib vao_attr(*program, "Position");
            vao_attr.Setup<Vec2f>();
            vao_attr.Enable();
        }

        // Bind VBO "Color"
        vbo_color->Bind(Buffer::Target::Array);
        {
            Buffer::Data(Buffer::Target::Array, camera[camera_id].intensities(), BufferUsage::StreamDraw);

            VertexArrayAttrib vao_attr(*program, "Color");
            vao_attr.Setup<Vec1f>();
            vao_attr.Enable();
        }


        // Draw all elements
        Context::DrawArrays(PrimitiveType::Points, 0, camera[camera_id].size());
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

    Context::Clear().DepthBuffer();
    glClearColor(0.0f, 0.2f, 0.9f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack & mv = Stacks::modelview();

    Context::Disable(Capability::CullFace);
    glDisable(GL_CULL_FACE);

    /*
    mv.withPush([&]
    {
        mv.rotate(elevation, glm::vec3(1.0, 0.0, 0.0));

        mv.rotate(240 * DEGREES_TO_RADIANS, glm::vec3(0.0, 1.0, 0.0));
        mv.rotate(azimuth, glm::vec3(0.0, 1.0, 0.0));

        mv.scale(10.1f);
        drawEvents(3);
    });

    mv.withPush([&]
    {
        mv.rotate(elevation, glm::vec3(1.0, 0.0, 0.0));

        mv.rotate(120 * DEGREES_TO_RADIANS, glm::vec3(0.0, 1.0, 0.0));
        mv.rotate(azimuth, glm::vec3(0.0, 1.0, 0.0));

        mv.scale(10.1f);
        drawEvents(3);
    });
    */

    mv.withPush([&]
    {
        mv.rotate(elevation, glm::vec3(1.0, 0.0, 0.0));
        mv.rotate(azimuth + 90 * DEGREES_TO_RADIANS, glm::vec3(0.0, 1.0, 0.0));

        mv.scale(10.1f);
        drawEvents(0);
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
