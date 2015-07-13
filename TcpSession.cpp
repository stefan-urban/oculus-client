
#include "TcpSession.hpp"
#include "vendor/dispatcher/Dispatcher.hpp"
#include "vendor/oculus-server/Message_JoystickEvent.hpp"

#include <string>


void TcpSession::event(DispatcherEvent* event)
{
    auto msg = Message_JoystickEvent();
    msg.unserialize(event->data());

    deliver(&msg);
}

void TcpSession::deliver(Message *msg)
{
    // Insert type
    unsigned char type = msg->get_type();
    std::vector<unsigned char> data = msg->serialize();

    data.insert(data.begin(), type);

    // Insert message length
    unsigned long body_size = data.size();

    for (size_t i = 0; i < header_length; i++)
    {
        data.insert(data.begin(), (body_size >> (i*8)) & 0xFF);
    }

    // Send message
    io_service_.post(
        [this, data]()
        {
            bool write_in_progress = !write_buffer_.empty();

            write_buffer_.push_back(data);

            if (!write_in_progress)
            {
                do_write();
            }
        });
}

void TcpSession::close()
{
    io_service_.post([this]() { socket_.close(); });
}

void TcpSession::do_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    boost::asio::async_connect(socket_, endpoint_iterator,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator)
        {
            if (!ec)
            {
                do_read_header();
            }
        });
}

void TcpSession::do_read_header()
{

    boost::asio::async_read(socket_, boost::asio::buffer(read_header_),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec || read_header_.size() != header_length)
            {
                unsigned long body_size = 0;

                for (size_t i = 0; i < header_length; i++)
                {
                    body_size |= read_header_[i] << ((header_length - i - 1) * 8);
                }

                read_body_.resize(body_size);

                // Now we know how long the message is, finally read it
                do_read_body();
            }
            else
            {
                socket_.close();
            }
        });
}

void TcpSession::do_read_body()
{
    boost::asio::async_read(socket_, boost::asio::buffer(read_body_, read_body_.size()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                // Determine type
                unsigned char type = read_body_[0];

                // Erase type from data block
                read_body_.erase(read_body_.begin());

                // Pack new event and dispatch it
                auto e = DispatcherEvent(type, &read_body_);
                dispatcher_->dispatch(&e);

                // And go back to reading the header
                do_read_header();
            }
            else
            {
                socket_.close();
            }
        });
}

void TcpSession::do_write()
{
    boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_.front()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                // Delete transmitted message
                write_buffer_.pop_front();

                // Continue transmitting if there are still messages left
                if (!write_buffer_.empty())
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
