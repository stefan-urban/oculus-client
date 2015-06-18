
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "EdvsImage.hpp"
#include "TcpClient.hpp"

#include "PhotoSphereExample.h"


#define DEBUG 1

// Create io service
boost::asio::io_service io_service;


void edvs_client_app(int argc, char* argv[])
{
    if (argc != 3)
    {
        return;
    }

    // Event handler (will be images)
    EdvsImage images[7];

    // Setup TCP connection
    boost::asio::ip::tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    TcpClient c(io_service, endpoint_iterator, images);

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
        result = PhotoSphereExample().run();
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
