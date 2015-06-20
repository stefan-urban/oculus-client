#include "EdvsRiftApp.h"
#include "Common.h"

using namespace oglplus;



void EdvsRiftApp::drawSphere()
{
    static ProgramPtr program = oria::loadProgram("./resources/Textured.vs", "./resources/Textured.fs");
    static ShapeWrapperPtr geometry = oria::loadShape({ "Position", "TexCoord" }, Resource::MESHES_SPHERE_CTM, program);

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

void EdvsRiftApp::renderScene(/*const glm::mat4 & projection, const glm::mat4 & headPose*/)
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
        case GLFW_KEY_G:
            projection_scale += 0.1f;
            return;
        case GLFW_KEY_H:
            projection_scale -= 0.1f;
            return;
        case GLFW_KEY_I:
            trans += 1.f;
            return;
        case GLFW_KEY_O:
            trans -= 1.f;
            return;
        case GLFW_KEY_Z:
            trans -= 5.f;
            return;
        case GLFW_KEY_E:
            fontsize += 2.f;
            return;
        case GLFW_KEY_W:
            fontsize -= 2.f;
            return;
        }
    }

    RiftApp::onKey(key, scancode, action, mods);
}
