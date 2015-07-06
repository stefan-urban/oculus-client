
#ifndef TCPSESSION_H
#define TCPSESSION_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "vendor/dispatcher/Dispatcher.hpp"
#include "vendor/oculus-server/Message.hpp"


class TcpSession
{
public:
    enum { header_length = 4 };

    TcpSession(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator endpoint_iterator, Dispatcher *dispatcher)
      : io_service_(io_service)
      , socket_(io_service)
      , dispatcher_(dispatcher)
    {
        do_connect(endpoint_iterator);
        read_header_.resize(header_length);
    }

  void deliver(Message *msg);
  void close();

private:
  void do_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
  void do_read_header();
  void do_read_body();
  void do_write();

private:
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::socket socket_;

    Dispatcher *dispatcher_;

    std::vector<unsigned char> read_header_;
    std::vector<unsigned char> read_body_;
    std::deque<std::vector<unsigned char>> write_buffer_;
};


#endif // TCPSESSION_H
