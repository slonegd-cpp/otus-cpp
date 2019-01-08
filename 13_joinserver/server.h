#pragma once

#include "session.h"

std::mutex mtx;

class Server : public std::enable_shared_from_this<Server>
{
public:
   Server (
      boost::asio::io_service& service,
      const boost::asio::ip::tcp::endpoint& endpoint
   ) 
      : acceptor (service, endpoint),
        socket   (service)
   {
      start();
   }


   void start()
   {
      std::lock_guard<std::mutex> lock(mtx);
      acceptor.async_accept (
         socket,
         [this] (boost::system::error_code error) {
            if (not error)
               std::make_shared<Session>(std::move(socket))->read_line();
            start();
         }
      );
   }


private:
   boost::asio::ip::tcp::acceptor acceptor;
   boost::asio::ip::tcp::socket   socket;
};