#ifndef EDVSIMAGEHANDLER_HPP
#define EDVSIMAGEHANDLER_HPP

#include <cstddef>
#include <boost/thread.hpp>

#include "EdvsCamera.hpp"
#include "vendor/dispatcher/Dispatcher.hpp"


class EdvsImageHandler : public DispatcherListener
{
public:
    EdvsImageHandler(boost::mutex *mutex)
        : mutex_(mutex)
    {
    }

    EdvsCamera *camera(size_t id)
    {
        return &(cameras_[id]);
    }

    void event(DispatcherEvent event);

private:
    EdvsCamera cameras_[7];
    boost::mutex *mutex_;
};

#endif // EDVSIMAGEHANDLER_HPP
