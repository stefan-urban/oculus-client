#include "Common.h"
#include "EdvsRiftApp.h"

using namespace oglplus;


void EdvsRiftApp::initGl()
{
    RiftApp::initGl();
}

void EdvsRiftApp::update()
{
    RiftApp::update();
}

void EdvsRiftApp::drawSphere()
{
    static ProgramPtr program = oria::loadProgram("./resources/sphere.vs", "./resources/sphere.fs");
    static ShapeWrapperPtr geometry = ShapeWrapperPtr(new shapes::ShapeWrapper({ "Position" }, shapes::ObjMesh(mesh_input.stream), *program));

    Platform::addShutdownHook([]
    {
        program.reset();
        geometry.reset();
    });

    MatrixStack & mv = Stacks::modelview();
    mv.withPush([&]
    {
        // Invert the sphere to see its insides
        mv.scale(vec3(-1));

        program->Use();

        Mat4Uniform(*program, "ModelView").Set(Stacks::modelview().top());
        Mat4Uniform(*program, "Projection").Set(Stacks::projection().top());

        // FoVs from binary shape file! Do not change if you do not know what you're doing!
        //oglplus::Uniform<float>(*program, "fov_x_start").Set(0);
        //oglplus::Uniform<float>(*program, "fov_x_end").Set(60);
        //oglplus::Uniform<float>(*program, "fov_y_start").Set(-30);
        //oglplus::Uniform<float>(*program, "fov_y_end").Set(30);

        geometry->Use();
        geometry->Draw();

        oglplus::NoProgram().Bind();
        oglplus::NoVertexArray().Bind();
    });

    //oglplus::DefaultTexture().Bind(oglplus::Texture::Target::_2D);
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

    mv.withPush([&]
    {
        //mv.scale(projection_scale + 0.1f);

        mv.preTranslate(glm::vec3(0, 0, 0.f + trans));

        mv.scale(10.1f);
        mv.rotate(-90.0 * DEGREES_TO_RADIANS + rotation, glm::vec3(0.0, 1.0, 0.0));
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
            projection_scale += 1.f;
            return;
        case GLFW_KEY_L:
            projection_scale -= 1.f;
            return;
        case GLFW_KEY_I:
            trans += 1.f;
            return;
        case GLFW_KEY_K:
            trans -= 1.f;
            return;
        case GLFW_KEY_U:
            rotation += 15.f * DEGREES_TO_RADIANS;
            return;
        case GLFW_KEY_J:
            rotation -= 15.f * DEGREES_TO_RADIANS;
            return;
        }
    }

    RiftApp::onKey(key, scancode, action, mods);
}
