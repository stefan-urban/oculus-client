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

#pragma once


#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <stdexcept>
#include <system_error>


#define HAVE_OPENCV 1

#define __STDC_FORMAT_MACROS 1
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#define OGLPLUS_USE_GLEW 1
#define OGLPLUS_USE_GLCOREARB_H 0
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/interop/glm.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>
#include <oglplus/bound/buffer.hpp>
#include <oglplus/shapes/sphere.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/error/prog_var.hpp>
#include <oglplus/error/program.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/opt/application.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/polar_coordinates.hpp>

// Most commonly used
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

// Calc ration
inline float aspect(const glm::uvec2 & v)
{
    return (float)v.x / (float)v.y;
}

#include <GLFW/glfw3.h>

// TODO: remove
#include <resources/Resources.h>


// Make sure some function will be executed at application exit
typedef std::function<void()> Lambda;
typedef std::list<Lambda> LambdaList;

class Finally
{
private:
    Lambda function;

public:
    Finally(Lambda function)
        : function(function)
    {
    }

    virtual ~Finally()
    {
        function();
    }
};


#include "Platform.h"
#include "Utils.h"

#include "rendering/Lights.h"
#include "rendering/MatrixStack.h"
#include "rendering/State.h"
#include "rendering/Colors.h"
#include "rendering/Vectors.h"

#include "opengl/Constants.h"
#include "opengl/Textures.h"
#include "opengl/Shaders.h"
#include "opengl/Framebuffer.h"
#include "opengl/GlUtils.h"

#include "glfw/GlfwUtils.h"
#include "glfw/GlfwApp.h"

#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

#include "ovr/OvrUtils.h"
#include "ovr/RiftManagerApp.h"
#include "ovr/RiftGlfwApp.h"
#include "ovr/RiftApp.h"
#include "ovr/RiftRenderingApp.h"

#ifndef PI
#define PI 3.14159265f
#endif

#ifndef HALF_PI
#define HALF_PI (PI / 2.0f)
#endif

#ifndef TWO_PI
#define TWO_PI (PI * 2.0f)
#endif

#ifndef RADIANS_TO_DEGREES
#define RADIANS_TO_DEGREES (180.0f / PI)
#endif

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS (PI / 180.0f)
#endif

