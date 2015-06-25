#include "Common.h"
#include "EdvsRiftApp.h"

#include <random>

using namespace oglplus;


std::vector<Vector<float, 3>> camera_intensity;

void EdvsRiftApp::initGl()
{
    RiftApp::initGl();

    intensities_buffer = BufferPtr(new Buffer());
}

void EdvsRiftApp::update()
{
    RiftApp::update();

    camera_intensity.clear();

    for (int i = 0; i < 10; i++)
    {
        camera_intensity.push_back(Vector<float, 3>(rand() % 128, rand() % 128, 1.0));
    }

    camera_intensity.push_back(Vector<float, 3>(-1.0, 0.0, 0.0));
}

void EdvsRiftApp::drawSphere()
{
    static ProgramPtr program = oria::loadProgram("./resources/sphere.vs", "./resources/sphere.fs");
    static ShapeWrapperPtr geometry = ShapeWrapperPtr(new shapes::ShapeWrapper({ "Position" }, shapes::ObjMesh(mesh_input.stream), *program));
    //static ShapeWrapperPtr geometry = ShapeWrapperPtr(new shapes::ShapeWrapper({ "Position" }, shapes::Screen(), *program));

    /*
    //
    vao = VertexArrayPtr(new VertexArray());
    vao->Bind();

    BufferPtr indexBuffer;
    indexBuffer = BufferPtr(new Buffer());
    indexBuffer->Bind(Buffer::Target::ElementArray);

    std::vector<GLfloat> indexData(5);
    Buffer::Data(Buffer::Target::ElementArray, indexData);

    VertexArrayAttrib(oria::Layout::Attribute::Color)
      .Pointer(3, DataType::Float, false, 5, 0)
      .Enable();
    */

    //
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

        //oglplus::Uniform<Vector<float, 3>>(*program, "intensity_map").Set(camera_intensity);

        std::vector<float> intensities(129*129);

        intensities_buffer->Bind(Buffer::Target::Array);
        {
            // Upload the data
            Buffer::Data(Buffer::Target::Array, intensities);

            // setup the vertex attribs array for the intensities
            VertexArrayAttrib(*program, "Color").Setup<GLfloat>(intensities.size()).Enable();
        }



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
