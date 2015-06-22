#include "EdvsRiftApp.h"
#include "Common.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

using namespace oglplus;


float fov_x;
float fov_y;

std::vector<float> x_start;
std::vector<float> y_start;

void EdvsRiftApp::initGl()
{
    RiftApp::initGl();

    fov_x = 60.0;
    fov_y = 60.0;

    for (int i = 0; i < 128; i++)
    {
        x_start.push_back((((float) i / 128.0) - 0.5) * fov_x);
        y_start.push_back((((float) i / 128.0) - 0.5) * fov_y);
    }

}

void EdvsRiftApp::update()
{
    RiftApp::update();
}

void EdvsRiftApp::drawSphere()
{
    static ProgramPtr program = oria::loadProgram("./resources/sphere.vs", "./resources/sphere.fs");
    static ShapeWrapperPtr geometry = oria::loadSphere({ "Position", "TexCoord" }, program);

    Platform::addShutdownHook([]
    {
        program.reset();
        geometry.reset();
    });

    MatrixStack & mv = Stacks::modelview();
    mv.withPush([&]
    {
        // Invert the sphere to see its insides
        mv.scale(vec3(1));
        oria::renderGeometry(geometry, program, LambdaList(
        {
            [&] {
                oglplus::Uniform<float>(*program, "x_start").Set(x_start);
                oglplus::Uniform<float>(*program, "y_start").Set(y_start);
            }
        }));
    });

    //oglplus::DefaultTexture().Bind(oglplus::Texture::Target::_2D);
}

void EdvsRiftApp::renderScene()
{
    using namespace oglplus;

    Context::Clear().DepthBuffer();
    glClearColor(0.0f, 0.2f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack & mv = Stacks::modelview();

    Context::Disable(Capability::CullFace);
    glDisable(GL_CULL_FACE);

    mv.withPush([&]
    {
        mv.scale(projection_scale);
        mv.preTranslate(glm::vec3(0, 0, -5.f + trans));
        drawSphere();
    });

    Context::Enable(Capability::CullFace);
}

void EdvsRiftApp::onKey(int key, int scancode, int action, int mods)
{
    if (GLFW_PRESS == action)
    {
        switch (key)
        {
        case GLFW_KEY_O:
            projection_scale += 0.1f;
            return;
        case GLFW_KEY_L:
            projection_scale -= 0.1f;
            return;
        case GLFW_KEY_I:
            trans += 1.f;
            return;
        case GLFW_KEY_K:
            trans -= 1.f;
            return;
        case GLFW_KEY_U:
            fontsize += 2.f;
            return;
        case GLFW_KEY_J:
            fontsize -= 2.f;
            return;
        }
    }

    RiftApp::onKey(key, scancode, action, mods);
}
