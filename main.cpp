
#include <iostream>
#include <string>
#include <cmath>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Common.h"
#include "EdvsEventHandler.hpp"
#include "EdvsEventLogger.hpp"
#include "EdvsRiftApp.h"
#include "TcpClient.hpp"
#include "JoystickEventHandler.hpp"
#include "vendor/edvstools/Edvs/EventStream.hpp"
#include "vendor/joystick/joystick.hh"
#include "vendor/dispatcher/Dispatcher.hpp"
#include "vendor/oculus-server/Message_RobotCommand.hpp"
#include "vendor/oculus-server/Message_EventCollection2.hpp"

#include "PhotoSphereExample.h"


// 10.162.177.202 4000
// 10.162.242.209 4000


// Global stop bit
int global_stop = 0;

boost::asio::io_service io_service;
boost::mutex mutex;

int edvs_images_app(EdvsEventHandler *edvs_event_handler)
{
    while (global_stop == 0)
    {
        edvs_event_handler->update();

        Platform::sleepMillis(30);
    }

    return 0;
}

int edvs_logging_app(EdvsEventLogger *edvs_event_logger)
{
    while (global_stop == 0)
    {
        edvs_event_logger->update();

        Platform::sleepMillis(1000);
    }

    return 0;
}

int joystick_app(TcpClient *tcp_client)
{
    auto joystick = Joystick("/dev/input/js0");
    auto event_handler = JoystickEventHandler(&joystick);

    while (global_stop == 0)
    {
        while (!joystick.isFound())
        {
            Platform::sleepMillis(500);
            joystick.reconnect();

            if (global_stop)
            {
                return 0;
            }
        }

        event_handler.handle_events();

        // Calculate direction (x-y-system, y is looking forward)
        int axis_x = event_handler.axis(0);
        int axis_y = event_handler.axis(1);

        // Normalize vectors to -1 to +1
        float max_val = 26000.0;

        float direction_x = -1.0 * ((float) axis_x) / max_val;
        float direction_y = -1.0 * ((float) axis_y) / max_val;

        // Limit to absolute value of one
        direction_x = direction_x > 1.0f ? 1.0f : direction_x;
        direction_x = direction_x < -1.f ? -1.f : direction_x;
        direction_y = direction_y > 1.0f ? 1.0f : direction_y;
        direction_y = direction_y < -1.f ? -1.f : direction_y;

        // Direction is the angle offset from "forward"
        float direction = std::atan2(direction_x, direction_y);

        if (direction < 0.0)
        {
            direction = TWO_PI + direction;
        }

        // Speed is simply the length of the vector, normalized to -1 to 1
        float speed = sqrt(0.5 * (direction_x * direction_x + direction_y * direction_y));

        // Correction, so that the controller allows speed 1.0 in every direction
        float tmp = (std::sqrt(2.0) - 1.0) / 2.0;
        float fac = std::cos(4*direction) * tmp + 1.0 + tmp;
        speed *= fac;


        // Transmit changes
        auto msg = Message_RobotCommand(direction, speed);
        tcp_client->deliver(&msg);


        Platform::sleepMillis(100);
    }

    return 0;
}

int oculus_rift_app(EdvsEventHandler *edvs_event_handler, Dispatcher *dispatcher)
{
    if (!ovr_Initialize())
    {
        SAY_ERR("Failed to initialize the Oculus SDK");
        global_stop = 1;
        return -1;
    }

    int result = -1;

    try
    {
        EdvsRiftApp rift_app(edvs_event_handler, &mutex, dispatcher);
        result = rift_app.run();
    }
    catch (std::exception & error)
    {
        SAY_ERR(error.what());
    }
    catch (std::string & error)
    {
        SAY_ERR(error.c_str());
    }

    global_stop = 1;
    io_service.stop();

    ovr_Shutdown();

    return result;
}

int main(int argc, char* argv[])
{
    std::cout << "oculus-client v1" << std::endl;

    if (argc != 3)
    {
        std::cout << "usage: oculus-client ip port" << std::endl;
        return 0;
    }

    // Single eDVS camera images
    auto edvs_event_handler = EdvsEventHandler(&mutex);
    auto edvs_event_logger = EdvsEventLogger();


    // Setup dispatcher
    auto dispatcher = Dispatcher();

    dispatcher.addListener(&edvs_event_handler, Message_EventCollection2::type_id);
    dispatcher.addListener(&edvs_event_logger, Message_EventCollection2::type_id);


    // TCP client connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    //auto endpoint_iterator = resolver.resolve({ "192.168.0.133", "4000" });

    TcpClient tcp_client(io_service, endpoint_iterator, &dispatcher);


    boost::thread jsa(joystick_app, &tcp_client);
    boost::thread ora(oculus_rift_app, &edvs_event_handler, &dispatcher);
    boost::thread eia(edvs_images_app, &edvs_event_handler);
    boost::thread ela(edvs_logging_app, &edvs_event_logger);


    // Start client
    io_service.run();

    tcp_client.close();


    jsa.join();
    ora.join();
    eia.join();
    ela.join();

    return 0;
}

