#include "EdvsCamera.hpp"

EdvsCamera::EdvsCamera()
{
    return;

    for (size_t i = 0; i < 6; i++)
    {
        GLfloat parity = i % 2 ? -1.0 : 1.0;
        unsigned long long time = 0;

        add(parity, std::pair<int, int>(20 + 2*i, 64), time);
    }
}

void EdvsCamera::add(float parity, std::pair<int, int> position, unsigned long long time)
{
    // Some plausibility checks
    if (parity > 1.0f ||
        parity < -1.0f ||
        position.first < 0 ||
        position.first > 128 ||
        position.second < 0 ||
        position.second > 128)
    {
        return;
    }

    indices_.push_back((GLuint) parity_.size());
    position_.push_back(((GLfloat)position.first) / 128.0);
    position_.push_back(((GLfloat)position.second) / 128.0);
    parity_.push_back((GLfloat) parity);
    time_.push_back(time);

    //std::cout << "size now: " << position_.size() << std::endl;
}

void EdvsCamera::clear()
{
    indices_.clear();
    position_.clear();
    parity_.clear();
    time_.clear();
}
