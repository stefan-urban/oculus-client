#include "Common.h"
#include "EdvsRiftApp.h"

#include <random>

using namespace oglplus;


void EdvsRiftApp::initGl()
{
    RiftApp::initGl();
}

void EdvsRiftApp::update()
{
    RiftApp::update();
}

TexturePtr EdvsRiftApp::loadImage()
{
    // Create context
    TexturePtr texture(new Texture());

    Platform::addShutdownHook([&]
    {
        texture.reset();
    });

    // Setup texture in context
    Context::Bound(TextureTarget::_2D, *texture)
        .MagFilter(TextureMagFilter::Nearest)
        .MinFilter(TextureMinFilter::Nearest)
        .Anisotropy(2.0f);


    // Create checkerboard mat
    int width = 64;
    int height = 128;
    int N = 3;

    GLubyte mat[width * height * N];

    for (int i = 0; i < 1; i++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                mat[y + x*height + i*(width*height)] = ((x % 2) ^ (y % 2)) * 255;

                //  mat[y + x*height + i*(width*height)] = 0;
            }
        }
    }

    //mat[2] = 255;

    // Bind texture
    texture->Bind(Texture::Target::_2D);

    // Assign data to texture
    Context::Bound(TextureTarget::_2D, *texture)
        .Image(images::Image(width, height, 1, N, mat));


    return texture;
}

void EdvsRiftApp::drawSphere()
{
    static ProgramPtr program = oria::loadProgram("./resources/sphere.vs", "./resources/sphere.fs");
    static ShapeWrapperPtr geometry = ShapeWrapperPtr(new shapes::ShapeWrapper({ "Position" }, shapes::ObjMesh(mesh_input.stream), *program));
    TexturePtr tex = loadImage();

    Platform::addShutdownHook([]
    {
        program.reset();
        geometry.reset();
        //tex.reset();
    });


    tex->Bind(Texture::Target::_2D);
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

    oglplus::DefaultTexture().Bind(oglplus::Texture::Target::_2D);
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
