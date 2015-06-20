
#include "TcpClient.hpp"
#include "vendor/oculus-server/TcpMessage.hpp"
#include "vendor/oculus-server/Message_EventCollection.hpp"


TcpClient::TcpClient(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator, std::vector<EdvsImage> *images)
  : io_service_(io_service),
    socket_(io_service)
{
    images_ = *images;

    do_connect(endpoint_iterator);
}

void TcpClient::write(const TcpMessage& msg)
{
  io_service_.post(
      [this, msg]()
      {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
          do_write();
        }
      });
}

void TcpClient::close()
{
  io_service_.post([this]() { socket_.close(); });
}

void TcpClient::do_connect(tcp::resolver::iterator endpoint_iterator)
{
  boost::asio::async_connect(socket_, endpoint_iterator,
      [this](boost::system::error_code ec, tcp::resolver::iterator)
      {
        if (!ec)
        {
          do_read_header();
        }
      });
}

void TcpClient::do_read_header()
{
  boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.data(), TcpMessage::header_length),
      [this](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec && read_msg_.decode_header())
        {
          do_read_body();
        }
        else
        {
          socket_.close();
        }
      });
}

void TcpClient::do_read_body()
{
  boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
            std::string const data = std::string(read_msg_.body());

            Message_EventCollection msg_events;
            msg_events.unserialize(&data);

            for(Edvs::Event& e : msg_events.events())
            {
                try
                {
                    images_[e.id].handle_event(e);
                }
                catch (std::out_of_range& /* err */)
                {
                    // Good heavens, what an unpleasant surprise
                }
            }

            do_read_header();
        }
        else
        {
          socket_.close();
        }
      });
}

void TcpClient::do_write()
{
  boost::asio::async_write(socket_,
      boost::asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
      [this](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          write_msgs_.pop_front();
          if (!write_msgs_.empty())
          {
            do_write();
          }
        }
        else
        {
          socket_.close();
        }
      });
}
