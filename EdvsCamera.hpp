#ifndef EDVSCAMERA_H
#define EDVSCAMERA_H

#include "Common.h"
#include "vendor/dispatcher/Dispatcher.hpp"

#include <time.h>
#include <random>
#include <utility>
#include <vector>

class EdvsCamera
{
public:
    EdvsCamera();

    void add(float parity, std::pair<int, int> position, unsigned long long time);
    void clear();

    std::vector<GLfloat> positions()
    {
        return position_;
    }

    std::vector<GLuint> indices()
    {
        return indices_;
    }

    std::vector<GLfloat> intensities()
    {
        return parity_;
    }

    size_t size()
    {
        return position_.size();
    }

private:
    std::vector<GLfloat> parity_;
    std::vector<GLfloat> position_;
    std::vector<GLuint> indices_;
    std::vector<unsigned long long> time_;
};

#endif // EDVSCAMERA_H
