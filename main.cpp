
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "EdvsImage.hpp"
#include "TcpClient.hpp"

#include "EdvsRiftApp.h"
#include "PhotoSphereExample.h"


#define DEBUG 1

// Create io service
boost::asio::io_service io_service;


// Single eDVS camera images
std::vector<Edvs::Event> events;


void edvs_client_app(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "TCP connection failed!" << std::endl;
        return;
    }

    // Setup TCP connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    TcpClient c(io_service, endpoint_iterator, &events);

    // Start client
    io_service.run();

    c.close();
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
        EdvsRiftApp rift_app(&events);
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
