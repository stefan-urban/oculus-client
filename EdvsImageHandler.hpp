#ifndef EDVSIMAGEHANDLER_HPP
#define EDVSIMAGEHANDLER_HPP

#include "vendor/dispatcher/Dispatcher.hpp"
#include "EdvsImage.hpp"

class EdvsImageHandler : public DispatcherListener
{
public:
    EdvsImage *images()
    {
        return images_;
    }

    void event(DispatcherEvent event);

private:
    EdvsImage images_[7];
};

#endif // EDVSIMAGEHANDLER_HPP
