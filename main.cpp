
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "EdvsImageHandler.hpp"
#include "EdvsRiftApp.h"
#include "TcpClient.hpp"
#include "JoystickEventHandler.hpp"
#include "vendor/edvstools/Edvs/EventStream.hpp"
#include "vendor/joystick/joystick.hh"
#include "vendor/dispatcher/Dispatcher.hpp"

#include "PhotoSphereExample.h"


// Global stop bit
int global_stop = 0;

int joystick_app(TcpClient *tcp_client)
{
    auto joystick = new Joystick("/dev/input/js0");
    auto event_handler = JoystickEventHandler(joystick);

    while (global_stop == 0)
    {
        while (!joystick->isFound())
        {
            Platform::sleepMillis(500);
            joystick->reconnect();

            if (global_stop)
            {
                return 0;
            }
        }

        event_handler.handle_events();

        // Transmit changes
        auto msg = Message_JoystickState(event_handler.button_states(), event_handler.axis_states());

        std::string test = msg.serialize();

        TcpMessage tcpMsg;
        tcpMsg.message(&msg);

        tcp_client->deliver(tcpMsg);


        Platform::sleepMillis(100);
    }

    return 0;
}

int oculus_rift_app(EdvsImageHandler *image_handler)
{
    if (!ovr_Initialize())
    {
        SAY_ERR("Failed to initialize the Oculus SDK");
        return -1;
    }

    int result = -1;

    try
    {
        EdvsRiftApp rift_app(image_handler);
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

    ovr_Shutdown();

    return result;
}

int main(int argc, char* argv[])
{
    std::cout << "oculus-client v1" << std::endl;

    // Create io service
    boost::asio::io_service io_service;

    // Single eDVS camera images
    EdvsImageHandler image_handler;


    // Setup dispatcher
    auto dispatcher = new Dispatcher();

    dispatcher->addListener(&image_handler);



    // TCP client connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    //auto endpoint_iterator = resolver.resolve({ "192.168.0.133", "4000" });

    TcpClient tcp_client(io_service, endpoint_iterator, dispatcher);


    boost::thread jsa(joystick_app, &tcp_client);
    boost::thread ora(oculus_rift_app, &image_handler);


    // Start client
    io_service.run();

    tcp_client.close();


    jsa.join();
    ora.join();

    return 0;
}
