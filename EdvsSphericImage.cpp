#include "EdvsSphericImage.hpp"
#include "Common.h"

#if 0

namespace Attribute {
    enum {
        Position = 0,
        TexCoord0 = 1,
        Normal = 2,
        Color = 3,
        TexCoord1 = 4,
        InstanceTransform = 5,
    };
}


static const char * VERTEX_SHADER =
"#version 330\n"

"uniform mat4 ProjectionMatrix;"
"uniform mat4 CameraMatrix;"

"layout(location = 0) in vec4 Position;"
"layout(location = 2) in vec3 Normal;"
"layout(location = 5) in mat4 InstanceTransform;"

"out vec3 vertNormal;"

"void main(void)"
"{"
" mat4 ViewXfm = CameraMatrix * InstanceTransform;"
" vertNormal = Normal;"
" gl_Position = ProjectionMatrix * ViewXfm * Position;"
"}";

static const char * FRAGMENT_SHADER =
"#version 330\n"
"in vec3 vertNormal;"

"out vec4 fragColor;"

"void main(void)"
"{"
" vec3 color = vertNormal;"
" if (!all(equal(color, abs(color)))) {"
"   color = vec3(1.0) - abs(color);"
" }"
" fragColor = vec4(color, 1.0);"
"}";


EdvsSphericImage::EdvsSphericImage() : sphere({ "Position", "Normal"}, oglplus::shapes::Sphere())
{
    using namespace oglplus;

    try
    {
        // attach the shaders to the program
        prog.AttachShader(
            FragmentShader()
            .Source(GLSLSource(String(FRAGMENT_SHADER)))
            .Compile()
        );
        prog.AttachShader(
            VertexShader()
            .Source(GLSLSource(String(VERTEX_SHADER)))
            .Compile()
        );
        prog.Link();
    }
    catch (ProgramBuildError & err)
    {
        FAIL((const char*)err.what());
    }

    // link and use it
    prog.Use();

    vao = sphere.VAOForProgram(prog);
    vao.Bind();

    // Create a cube of cubes
    {
        std::vector<mat4> instance_positions;
        for (unsigned int z = 0; z < GRID_SIZE; ++z)
        {
            for (unsigned int y = 0; y < GRID_SIZE; ++y)
            {
                for (unsigned int x = 0; x < GRID_SIZE; ++x)
                {
                    int xpos = (x - (GRID_SIZE / 2)) * 2;
                    int ypos = (y - (GRID_SIZE / 2)) * 2;
                    int zpos = z * -2;
                    vec3 relativePosition = vec3(xpos, ypos, zpos);
                    instance_positions.push_back(glm::translate(glm::mat4(1.0f), relativePosition));
                }
            }
        }

        Context::Bound(Buffer::Target::Array, instances).Data(instance_positions);
        instanceCount = instance_positions.size();
        int stride = sizeof(mat4);
        for (int i = 0; i < 4; ++i)
        {
            VertexArrayAttrib instance_attr(prog, Attribute::InstanceTransform + i);
            size_t offset = sizeof(vec4) * i;
            instance_attr.Pointer(4, DataType::Float, false, stride, (void*)offset);
            instance_attr.Divisor(1);
            instance_attr.Enable();
        }
    }
}

void EdvsSphericImage::render(const mat4 & projection, const mat4 & modelview)
{
    using namespace oglplus;

    prog.Use();
    typedef oglplus::Uniform<mat4> Mat4Uniform;
    Mat4Uniform(prog, "ProjectionMatrix").Set(projection);
    Mat4Uniform(prog, "CameraMatrix").Set(modelview);
    vao.Bind();

    sphere.Draw(instanceCount);
}

#endif
