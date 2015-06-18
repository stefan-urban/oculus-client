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

#include "PhotoSphereExample.h"


void PhotoSphereExample::drawSphere()
{
    static ProgramPtr program = oria::loadProgram(Resource::SHADERS_TEXTURED_VS, Resource::SHADERS_TEXTURED_FS);
    static ShapeWrapperPtr geometry = oria::loadShape({ "Position", "TexCoord" }, Resource::MESHES_SPHERE_CTM, program);
    static TexturePtr t = loadAndPositionPhotoSphereImage(Resource::IMAGES_PANO_20140620_160351_JPG);

    Platform::addShutdownHook([]
    {
        program.reset();
        geometry.reset();
        t.reset();
    });

    t->Bind(oglplus::Texture::Target::_2D);
    MatrixStack & mv = Stacks::modelview();
    mv.withPush([&]
    {
        // Invert the sphere to see its insides
        mv.scale(vec3(-1));
        oria::renderGeometry(geometry, program);
    });

    oglplus::DefaultTexture().Bind(oglplus::Texture::Target::_2D);
}

void PhotoSphereExample::renderScene()
{
    using namespace oglplus;
    Context::Clear().DepthBuffer();
    glClear(GL_DEPTH_BUFFER_BIT);

    MatrixStack & mv = Stacks::modelview();

    Context::Disable(Capability::CullFace);
    glDisable(GL_CULL_FACE);
    mv.withPush([&]
    {
        mv.scale(50.0f);
        drawSphere();
    });

    Context::Enable(Capability::CullFace);
}

TexturePtr PhotoSphereExample::loadAndPositionPhotoSphereImage(Resource res)
{
    glm::uvec2 fullPanoSize;
    glm::uvec2 croppedImageSize;
    glm::uvec2 croppedImagePos;
    using namespace oglplus;

    TexturePtr texture(new Texture());
    Platform::addShutdownHook([&]
    {
        texture.reset();
    });

    auto v = Platform::getResourceByteVector(res);
    cv::Mat mat = cv::imdecode(v, CV_LOAD_IMAGE_COLOR);
    cv::cvtColor(mat, mat, CV_BGR2RGB);

    //cv::flip(mat, mat, 0);
    Context::Bound(TextureTarget::_2D, *texture)
    .MagFilter(TextureMagFilter::Linear)
    .MinFilter(TextureMinFilter::Linear);

    if (parseExifData(Platform::getResourceString(Resource::MISC_PANO_20140620_160351_EXIV), fullPanoSize, croppedImageSize, croppedImagePos))
    {
        // EXIF data parsed succesfully
        uchar *embedded = (uchar*)malloc(fullPanoSize.x * fullPanoSize.y * 3);
        insetImage(fullPanoSize, croppedImageSize, croppedImagePos, mat, embedded);
        Context::Bound(TextureTarget::_2D, *texture)
        .Image2D(images::Image(fullPanoSize.x, fullPanoSize.y, 1, 3, embedded));
        free(embedded);
    }
    else
    {
        // Failed to load EXIF data
        texture->Bind(Texture::Target::_2D);
        //      Context::PixelStore(PixelStorageMode::UnpackAlignment, 1);
        Context::Bound(TextureTarget::_2D, *texture)
        .Image2D(images::Image(mat.cols, mat.rows, 1, 3, mat.datastart));
    }

    return texture;
}

void PhotoSphereExample::insetImage(glm::uvec2 &fullPanoSize, glm::uvec2 &croppedImageSize, glm::uvec2 &croppedImagePos, cv::Mat &mat, uchar *out)
{
    memset(out, 84, fullPanoSize.x * fullPanoSize.y * 3);

    for (unsigned int y = 0; y < croppedImageSize.y; y++)
    {
        int srcRow = y * croppedImageSize.x * 3;
        int destRow = (croppedImagePos.y + y) * (fullPanoSize.x * 3);
        memcpy(out + destRow + croppedImagePos.x, mat.datastart + srcRow, croppedImageSize.x * 3);
    }
}

bool PhotoSphereExample::parseExifData(const std::string & exifData, glm::uvec2 &fullPanoSize, glm::uvec2 &croppedImageSize, glm::uvec2 &croppedImagePos)
{
    // Extract pano fields from temp file
    int fieldsFound = 0;
    std::string line;
    std::istringstream infile(exifData);
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string field, type, name;

        int value;
        if (iss >> field >> type >> name >> value)
        {
            if (field.compare("Xmp.GPano.FullPanoWidthPixels") == 0)
            {
                fullPanoSize.x = value;
                fieldsFound |= (1 << 0);
            }
            if (field.compare("Xmp.GPano.FullPanoHeightPixels") == 0)
            {
                fullPanoSize.y = value;
                fieldsFound |= (1 << 1);
            }
            if (field.compare("Xmp.GPano.CroppedAreaLeftPixels") == 0)
            {
                croppedImagePos.x = value;
                fieldsFound |= (1 << 2);
            }
            if (field.compare("Xmp.GPano.CroppedAreaTopPixels") == 0)
            {
                croppedImagePos.y = value;
                fieldsFound |= (1 << 3);
            }
            if (field.compare("Xmp.GPano.CroppedAreaImageWidthPixels") == 0)
            {
                croppedImageSize.x = value;
                fieldsFound |= (1 << 4);
            }
            if (field.compare("Xmp.GPano.CroppedAreaImageHeightPixels") == 0)
            {
                croppedImageSize.y = value;
                fieldsFound |= (1 << 5);
            }
        }
    }

    if (fieldsFound != ((1 << 6) - 1))
    {
        std::cout << "Didn't find expected XMP fields" << std::endl;
        return false;
    }

    return true;
}

bool PhotoSphereExample::loadExifDataFromImage(const std::string &pathToImage, glm::uvec2 &fullPanoSize, glm::uvec2 &croppedImageSize, glm::uvec2 &croppedImagePos)
{
    static const std::string pathToExiv2 = "C:\\bin\\exiv2\\exiv2.exe";
    static const std::string exiv2Params = "-pa print";
    static const std::string tempFile = "temp.txt";

    // Extract XMP data from image, write to temp file
    std::string command = pathToExiv2 + " " + exiv2Params + " " + pathToImage + " > " + tempFile;
    system(command.c_str());
    std::string exifData = oria::readFile(tempFile);

    if (parseExifData(exifData, fullPanoSize, croppedImageSize, croppedImagePos))
    {
        remove(tempFile.c_str());
        return true;
    }

    std::cout << "output has been left in " + tempFile << std::endl;
    return false;
}
