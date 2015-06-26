#include "Common.h"
#include "EdvsRiftApp.h"

#include <random>

using namespace oglplus;


void EdvsRiftApp::initGl()
{
    RiftApp::initGl();

    vbo = BufferPtr(new Buffer());
}

void EdvsRiftApp::update()
{
    RiftApp::update();
}

void EdvsRiftApp::drawSphere(int camera)
{
    static ProgramPtr program = oria::loadProgram("./resources/sphere.vs", "./resources/sphere.fs");
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


        // Make sure VAO is bound
        geometry->Use();


        // TODO: fill image into intensities matrix
        (*images_)[camera].update();

        // Every pixel is represented by 6 vertexes
        std::vector<GLfloat> intensities((*images_)[camera].size() * 6);

        for (size_t i = 0; i < intensities.size(); i++)
        {
            intensities[i] = (*images_)[camera].data(i / 6);
        }

        // Bind VBO
        vbo->Bind(Buffer::Target::Array);
        Buffer::Data(Buffer::Target::Array, intensities);

        // Setup attribute to access buffer in shader
        VertexArrayAttrib vbo_attr(*program, "Color");
        vbo_attr.Setup<GLfloat>(1);
        vbo_attr.Enable();

        // Draw all elements
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

    mv.withPush([&]
    {
        //mv.scale(projection_scale + 0.1f);

        mv.preTranslate(glm::vec3(0, 0, 0.f + trans));

        mv.scale(10.1f);
        mv.rotate(90.0 * DEGREES_TO_RADIANS + rotation, glm::vec3(0.0, 1.0, 0.0));
        drawSphere(3);
    });

    mv.withPush([&]
    {
        //mv.scale(projection_scale + 0.1f);

        mv.preTranslate(glm::vec3(0, 0, 0.f + trans));

        mv.scale(10.1f);
        mv.rotate(90.0 * DEGREES_TO_RADIANS - 70.0 * DEGREES_TO_RADIANS + rotation, glm::vec3(0.0, 1.0, 0.0));
        drawSphere(4);
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
