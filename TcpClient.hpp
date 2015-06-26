
//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "EdvsImage.hpp"
#include "vendor/oculus-server/TcpMessage.hpp"


using boost::asio::ip::tcp;

typedef std::deque<TcpMessage> TcpMessageQueue;

class TcpClient
{
public:
  TcpClient(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator, EdvsImage (*images)[7])
      : io_service_(io_service),
        socket_(io_service),
        images_(images)
    {
        do_connect(endpoint_iterator);
    }

  void write(const TcpMessage& msg);
  void close();

private:
  void do_connect(tcp::resolver::iterator endpoint_iterator);
  void do_read_header();
  void do_read_body();
  void do_write();

private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    TcpMessage read_msg_;
    TcpMessageQueue write_msgs_;
    EdvsImage (*images_)[7];
};


#endif // TCPCLIENT_H
