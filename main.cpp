
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <OVR_CAPI.h>

#include "EdvsImage.hpp"
#include "TcpClient.hpp"


void edvs_client_app(int argc, char* argv[])
{
    if (argc != 3)
    {
        return;
    }

    // Create io service
    boost::asio::io_service io_service;

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

void oculus_rift_app()
{
    ovrBool init_status = ovr_Initialize(nullptr);

    if (init_status)
    {
        ovrHmd hmd = ovrHmd_Create(0);

        if (hmd)
        {
            // Get more details about the HMD.
            ovrSizei resolution = hmd->Resolution;

            while(1)
            {

            }

            // Do something with the HMD.
            ovrHmd_Destroy(hmd);
        }

        ovr_Shutdown();
    }
}

int main(int argc, char* argv[])
{
    std::cout << "oculus-client v1" << std::endl;


    boost::thread eca(edvs_client_app, argc, argv);
    boost::thread ora(oculus_rift_app);

    while (1);

    eca.join();
    ora.join();

    return 0;
}
