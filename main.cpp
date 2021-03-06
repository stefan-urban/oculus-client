
#include <iostream>
#include <string>
#include <cmath>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Common.h"
#include "EdvsEventHandler.hpp"
#include "EdvsEventLogger.hpp"
#include "EdvsRiftApp.h"
#include "TcpSession.hpp"
#include "JoystickEventHandler.hpp"
#include "vendor/edvstools/Edvs/EventStream.hpp"
#include "vendor/joystick/joystick.hh"
#include "vendor/dispatcher/Dispatcher.hpp"
#include "InputEvent.hpp"

#include "vendor/oculus-server/Message_RobotCommand.hpp"
#include "vendor/oculus-server/Message_JoystickEvent.hpp"
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

int joystick_app(TcpSession *tcp_client, Dispatcher *dispatcher)
{
    auto joystick = Joystick("/dev/input/js0");
    auto event_handler = JoystickEventHandler(&joystick, dispatcher);

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
        int axis_a = event_handler.axis(2);

        int x_speed = (int) (((float)axis_x / 30000.0f) * 70.0f);
        int y_speed = (int) (((float)axis_y / -30000.0f) * 70.0f);
        int angular_speed = (int) (((float)axis_a / -30000.0f) * 70.0f);

        x_speed = x_speed > 70 ? 70 : x_speed;
        x_speed = x_speed < -70 ? -70 : x_speed;
        y_speed = y_speed > 70 ? 70 : y_speed;
        y_speed = y_speed < -70 ? -70 : y_speed;
        angular_speed = angular_speed > 70 ? 70 : angular_speed;
        angular_speed = angular_speed < -70 ? -70 : angular_speed;

        // Transmit changes
        auto msg = Message_RobotCommand(x_speed, y_speed, angular_speed);
        tcp_client->deliver(&msg);

        Platform::sleepMillis(200);
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
    //dispatcher.addListener(&edvs_event_logger, Message_EventCollection2::type_id);

    // TCP client connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    //auto endpoint_iterator = resolver.resolve({ "192.168.0.133", "4000" });

    TcpSession tcp_client(io_service, endpoint_iterator, &dispatcher);
    dispatcher.addListener(&tcp_client, Message_JoystickEvent::type_id);


    boost::thread jsa(joystick_app, &tcp_client, &dispatcher);
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

