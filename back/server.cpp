#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>
#include <boost/chrono/chrono.hpp>
#include <iostream>
#include <chrono>
#include <memory>
#include <map>
#include <string>
#include <ctime>

using boost::asio::local::stream_protocol;

class session : public std::enable_shared_from_this<session>
{
public:
    explicit session(
      stream_protocol::socket socket,
      std::map<std::string, std::time_t>& userAgent_)
            : socket_(std::move(socket)),
              timer_(socket_.get_io_service()),
              strand_(socket_.get_io_service()),
              userAgent(userAgent_)
    {
    }

    void run()
    {
        auto self(shared_from_this());
        boost::asio::spawn(strand_,
          [this, self](boost::asio::yield_context yield)
          {
              try
              {
                  char data_read[1024];
                  char data_write[1024];
                  for (;;)
                  {
                      timer_.expires_from_now(std::chrono::seconds(100));
                      std::size_t n = socket_.async_read_some(boost::asio::buffer(data_read), yield);
                      std::string user_agent_str(data_read);
                      //is this first UA entry?
                      if ( userAgent.find(user_agent_str) == userAgent.end() ) {
                          //set cuurent time as UA value
                          userAgent[user_agent_str] = std::time(nullptr);
                          std::strcpy(data_write, "Seems, that you are the first person, wisited via such browser! Great!");
                      } else {                        
                          //count difference between UA visit time (as days)
                          int days_passed = (std::time(nullptr) - userAgent[user_agent_str])/(60*60*24);
                          userAgent[user_agent_str] = std::time(nullptr);
                          switch(days_passed){
                            case 0:
                            case 1:
                              std::sprintf(
                                  data_write, 
                                  "Well you're not \"first-of-your-kind\", visitor with such browser. There was another one %s.", 
                                  days_passed ? "yestreday" : "today");
                              break;
                            default:
                                  std::sprintf(data_write, "Hi there. person on such browser visited about %d days ago.", days_passed);
                          }

                      }
                      boost::asio::async_write(socket_, boost::asio::buffer(data_write, strlen(data_write)), yield);
                  }
              }
              catch (std::exception& e)
              {
                  socket_.close();
                  timer_.cancel();
              }
          });

        boost::asio::spawn(strand_,
          [this, self](boost::asio::yield_context yield)
          {
              while (socket_.is_open())
              {
                  boost::system::error_code ignored_ec;
                  timer_.async_wait(yield[ignored_ec]);
                  if (timer_.expires_from_now() <= std::chrono::seconds(0))
                      socket_.close();
              }
          });
    }

private:
    stream_protocol::socket socket_;
    boost::asio::steady_timer timer_;
    boost::asio::io_service::strand strand_;
    std::map<std::string, std::time_t>& userAgent;

};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: echo_server <path_to_unix_socket>\n";
            return 1;
        }

        std::map<std::string, std::time_t> userAgent;
        boost::asio::io_service io_service;

        boost::asio::spawn(io_service,
          [&](boost::asio::yield_context yield)
          {
              ::unlink(argv[1]);
              stream_protocol::endpoint ep(argv[1]);

              stream_protocol::acceptor acceptor(io_service, ep);
              for (;;)
              {
                  boost::system::error_code ec;
                  stream_protocol::socket socket(io_service);
                  acceptor.async_accept(socket, yield[ec]);
                  if (!ec) std::make_shared<session>(std::move(socket),userAgent)->run();
              }
          });

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
  }