#include "Common.h"

// For some interaction with the Oculus SDK we'll need the native window
// handle from GLFW.  To get it we need to define a couple of macros
// (that depend on OS) and include an additional header

#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX

#include <GLFW/glfw3native.h>

namespace glfw
{
    void * getNativeWindowHandle(GLFWwindow * window)
    {
        void * nativeWindowHandle = nullptr;

        nativeWindowHandle = (void*)glfwGetX11Window(window);

        return nativeWindowHandle;
    }

    void * getNativeDisplay(GLFWwindow * window)
    {
        void * nativeDisplay = nullptr;

        nativeDisplay = (void*)glfwGetX11Display();

        return nativeDisplay;
    }


}
