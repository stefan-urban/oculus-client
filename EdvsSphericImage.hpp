#ifndef EDVSSPHERICIMAGE_H
#define EDVSSPHERICIMAGE_H

#if 0

#include "Common.h"

using namespace oglplus;


class EdvsSphericImage
{
public:
    EdvsSphericImage();
    void render(const mat4 & projection, const mat4 & modelview);

private:
    const unsigned int GRID_SIZE{ 3 };

    // Program
    oglplus::Program prog;
    oglplus::shapes::ShapeWrapper sphere;
    GLuint instanceCount;
    oglplus::VertexArray vao;
    oglplus::Buffer instances;


};

#endif

#endif // EDVSSPHERICIMAGE_H
