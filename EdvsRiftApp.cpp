#include "EdvsRiftApp.h"

void EdvsRiftApp::initGl() {
    RiftApp::initGl();
    glClearColor(0.0f, 0.2f, 0.5f, 0.0f);
    glClearDepth(1.0f);
    glDisable(GL_DITHER);
    glEnable(GL_DEPTH_TEST);
}

void EdvsRiftApp::renderScene()
{
    using namespace oglplus;

    Context::Clear().DepthBuffer();

    glClearColor(0.0f, 0.2f, 0.5f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    Context::Disable(Capability::CullFace);
    Context::Enable(Capability::CullFace);
}
