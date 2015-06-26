#include "EdvsImage.hpp"

#include <glm/vec2.hpp>
#include <cmath>


static glm::vec2 id2corrds(int id)
{
    return glm::vec2(
        id % 128,
        id / 128
    );
}

static int coords2id(int x, int y)
{
    return x + y * 128;
}

static int coords2id(glm::vec2 coords)
{
    return coords2id(coords.x, coords.y);
}


EdvsImage::EdvsImage()
{
    t_lastupdate = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 128; i++)
    {
        for (size_t j = 0; j < 128; j++)
        {
            image_[j + i * 128] = (float) ((i % 2) ^ (j % 2));
        }
    }
}

static int count = 0;

void EdvsImage::add_event(Edvs::Event *event)
{
    count++;

    int pixel_id = coords2id(event->y, 128 - event->x);

    image_[pixel_id] = event->parity ? 1.0 : -1.0;

    // TODO: time has to have an effect on start value
}

void EdvsImage::update()
{
    // Calculate transformation
    std::chrono::high_resolution_clock::time_point t_end = std::chrono::high_resolution_clock::now() ;
    std::chrono::duration<int,std::micro> duration( std::chrono::duration_cast<std::chrono::duration<int,std::micro>>(t_end - t_lastupdate) ) ;

    // Factor is depending only on time
    float factor = exp(-1 * (float) duration.count() / decay_);

    // Apply factor on every single pixel
    for (int i = 0; i < 128*128; i++)
    {
        image_[i] *= factor;
    }

    // Reset timer
    t_lastupdate = std::chrono::high_resolution_clock::now();
}

