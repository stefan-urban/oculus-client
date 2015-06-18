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

#include "Common.h"
#include <fstream>
#include <cstdio>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class PhotoSphereExample : public RiftApp
{

public:
    PhotoSphereExample()
    {
    }

    void drawSphere();
    void renderScene();

    /**
    * Call out to the exiv2 binary to retrieve the XMP fields encoded in the target image file.
    */
    static TexturePtr loadAndPositionPhotoSphereImage(Resource res);

    /**
    * Embed the image in mat into a larger frame.
    */
    static void insetImage(glm::uvec2 &fullPanoSize, glm::uvec2 &croppedImageSize, glm::uvec2 &croppedImagePos, cv::Mat &mat, uchar *out);
    static bool parseExifData(const std::string & exifData, glm::uvec2 &fullPanoSize, glm::uvec2 &croppedImageSize, glm::uvec2 &croppedImagePos);
    static bool loadExifDataFromImage(const std::string &pathToImage, glm::uvec2 &fullPanoSize, glm::uvec2 &croppedImageSize, glm::uvec2 &croppedImagePos);
};
