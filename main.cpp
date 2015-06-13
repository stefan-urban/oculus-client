
#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "TcpClient.hpp"


int main(int argc, char* argv[])
{
    std::cout << "oculus-client v1" << std::endl;

    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
        TcpClient c(io_service, endpoint_iterator);

        std::thread t([&io_service](){ io_service.run(); });

        char line[TcpMessage::max_body_length + 1];

        std::cout << " > ";
        while (std::cin.getline(line, TcpMessage::max_body_length + 1))
        {
            TcpMessage msg;
            c.write(msg);

            // Wait 10 ms
            usleep(10 * 1000);
            std::cout << " > ";
        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

  return 0;
}
