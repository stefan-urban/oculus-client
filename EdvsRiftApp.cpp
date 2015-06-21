#include "EdvsRiftApp.h"
#include "Common.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

using namespace oglplus;


void EdvsRiftApp::initGl()
{
    RiftApp::initGl();

    t_start = std::chrono::high_resolution_clock::now();
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
        mv.scale(vec3(-1));
        oria::renderGeometry(geometry, program);
    });

    //oglplus::DefaultTexture().Bind(oglplus::Texture::Target::_2D);
}

void EdvsRiftApp::drawEdvsEvent(glm::mat4 transform, float intensity)
{
    vec3 color;

    // Negative event
    if (intensity < 0.0 && intensity >= -1.0)
    {
        color = glm::vec3(0, intensity * -1.0, 0);
    }
    // Positive event
    else if (intensity > 0.0 && intensity <= 1.0)
    {
        color = glm::vec3(intensity, 0, 0);
    }

    static ProgramPtr program = oria::loadProgram("./resources/edvs_events.vs", "./resources/edvs_events.fs");
    static ShapeWrapperPtr geometry = oria::loadPlane(program, 1.0f);

    Platform::addShutdownHook([]
    {
        program.reset();
        geometry.reset();
    });

    MatrixStack & mv = Stacks::modelview();
    mv.withPush([&]
    {
        // Do rotation and translation
        mv.transform(transform);

        oria::renderGeometry(geometry, program, LambdaList(
        {
            [&] {
                oglplus::Uniform<vec4>(*program, "color").Set(glm::vec4(color, 0.0));
            }
        }));
    });
}

void EdvsRiftApp::renderScene(/*const glm::mat4 & projection, const glm::mat4 & headPose*/)
{
    using namespace oglplus;

    Context::Clear().DepthBuffer();
    glClearColor(0.0f, 0.2f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MatrixStack & mv = Stacks::modelview();

    Context::Disable(Capability::CullFace);
    glDisable(GL_CULL_FACE);

    // Draw camera one
    mv.withPush([&]
    {
        mv.scale(0.01 * projection_scale);
        mv.preTranslate(glm::vec3(0, 0, -5.f + trans));

        int N = 4;

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                // Polar coordinates
                //float theta = 0.0f + 0.1 * (float) i;
                //float roh = 0.0f + 0.1 * (float) i;

                //auto polar_coords = glm::vec2(theta, roh);

                // Conversion to euclidean coordinates (radius is assumed to be one)
                //auto euclidean_coords = glm::euclidean(polar_coords);


                // Convert to translation and rotation
                /*auto transform = glm::lookAt(
                    euclidean_coords,
                    glm::vec3(0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)
                );*/

                // glm::vec3((float) i * 2.0, (float) j * 2.0, 0)



                // Calculate transformation
                //auto t_now = std::chrono::high_resolution_clock::now();
                //float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

                //glm::mat4 trans;
                glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + (float) i * 2.0, 0.0f + (float) j * 2.0, 0.0f));

                drawEdvsEvent(trans, (float) (i - N/2 + 1) / 2.0 / N + (float) (j - N/2 + 1) / 2.0 / N);
            }
        }
    });


    mv.withPush([&]
    {
        //mv.scale(projection_scale);
        //drawSphere();

    });

    // Text
    //const std::string text = "teststring";
    //glm::vec3 cursor = glm::vec3(-0.5, -0.5, 0);
    //oria::renderString(text, cursor, fontsize, Resource::FONTS_INCONSOLATA_MEDIUM_SDFF);

    //Context::Enable(Capability::CullFace);

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
