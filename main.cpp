
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "EdvsImage.hpp"
#include "EdvsRiftApp.h"
#include "TcpClient.hpp"
#include "vendor/edvstools/Edvs/EventStream.hpp"

#include "PhotoSphereExample.h"


#define DEBUG 1

/**
  * Connection type
  */
#define CONNECTION_TYPE_TCP 1
#define CONNECTION_TYPE_SOCKET 2

#define CONNECTION_TYPE CONNECTION_TYPE_TCP


// Global stop bit
int global_stop = 0;

// Create io service
boost::asio::io_service io_service;

// Single eDVS camera images
EdvsImage images[7];


void edvs_client_app(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "TCP connection failed!" << std::endl;
        return;
    }

#if CONNECTION_TYPE == CONNECTION_TYPE_TCP
    // Setup TCP connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    //auto endpoint_iterator = resolver.resolve({ "192.168.0.133", "4000" });

    TcpClient c(io_service, endpoint_iterator, &images);

    // Start client
    io_service.run();

    c.close();

#elif CONNECTION_TYPE == CONNECTION_TYPE_SOCKET

    const std::vector<std::string> uris = {
        "192.168.0.133:7004",
        "192.168.0.133:7005"
    };
    auto stream = Edvs::OpenEventStream(uris);

    if (stream->is_live() && stream->is_open())
    {
        std::cout << "info: stream open" << std::endl;
    }
    else
    {
        std::cout << "error: stream NOT open" << std::endl;
    }

    while(global_stop == 0)
    {
        auto events = stream->read();

        for(Edvs::Event& e : events)
        {
            images[e.id].add_event(&e);
        }

        // Sleep for 10 ms
        usleep(20 * 1000);
    }

#else
    #error You must at least specify one way to transmit data.
#endif
}


int oculus_rift_app()
{
    if (!ovr_Initialize())
    {
        SAY_ERR("Failed to initialize the Oculus SDK");
        return -1;
    }

    int result = -1;

    try
    {
        EdvsRiftApp rift_app(&images);
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


    boost::thread eca(edvs_client_app, argc, argv);
    boost::thread ora(oculus_rift_app);


    eca.join();
    ora.join();

    return 0;
}
