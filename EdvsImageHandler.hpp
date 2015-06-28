#ifndef EDVSIMAGEHANDLER_HPP
#define EDVSIMAGEHANDLER_HPP

#include <cstddef>

#include "EdvsCamera.hpp"
#include "vendor/dispatcher/Dispatcher.hpp"


class EdvsImageHandler : public DispatcherListener
{
public:
    EdvsCamera *camera(size_t id)
    {
        return &(cameras_[id]);
    }

    void event(DispatcherEvent event);

private:
    EdvsCamera cameras_[7];
};

#endif // EDVSIMAGEHANDLER_HPP
