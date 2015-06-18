/************************************************************************************

Authors     :   Bradley Austin Davis <bdavis@saintandreas.org>
Copyright   :   Copyright Brad Davis. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/

#include "Common.h"

#ifdef _DEBUG
#define BRAD_DEBUG 1
#endif

namespace ovr
{
    /**
    * Build an extended mode window positioned to exactly overlay the OS desktop monitor
    * which corresponds to the Rift.
    */
    GLFWwindow * createExtendedModeWindow(glm::uvec2 & outSize, glm::ivec2 & outPosition)
    {
        // In extended desktop mode, we should be using the current resolution of the Rift.
        GLFWmonitor * monitor = glfw::getMonitorAtPosition(outPosition);
        if (nullptr != monitor)
        {
            auto mode = glfwGetVideoMode(monitor);
            outSize = glm::uvec2(mode->width, mode->height);
        }

        // If we're creating a desktop window, we strip off any window decorations
        // which might change the location of the rendered contents relative to the lenses.
        glfwWindowHint(GLFW_DECORATED, 0);
        return glfw::createFullscreenWindow(outSize, monitor);
    }

    /**
    * Build an OpenGL window, respecting the Rift's current display mode choice of
    * extended or direct HMD.
    */
    GLFWwindow * createRiftRenderingWindow(ovrHmd hmd, glm::uvec2 & outSize, glm::ivec2 & outPosition)
    {
        outPosition = glm::ivec2(hmd->WindowsPos.x, hmd->WindowsPos.y);
        outSize = glm::uvec2(hmd->Resolution.w, hmd->Resolution.h);

        return createExtendedModeWindow(outSize, outPosition);
    }

}

namespace oria
{

// Returns true if the HSW needed to be removed from display, else false.
    bool clearHSW(ovrHmd hmd)
    {
        static bool dismissedHsw = false;

        if (!dismissedHsw)
        {
            ovrHSWDisplayState hswDisplayState;
            ovrHmd_GetHSWDisplayState(hmd, &hswDisplayState);
            if (hswDisplayState.Displayed)
            {
                ovrHmd_DismissHSWDisplay(hmd);
                return true;
            }
            else
            {
                dismissedHsw = true;
            }
        }
        return !dismissedHsw;
    }

}
