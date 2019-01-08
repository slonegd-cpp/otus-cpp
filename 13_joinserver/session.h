#pragma once

#include <vector>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "database.h"
#include "threadpool.h"


extern DataBase database;


class Session;


struct Args {
   std::string command;
   std::shared_ptr<Session> session;
};
void database_worker(Args);



class Session : public std::enable_shared_from_this<Session>
{
   boost::asio::ip::tcp::socket socket;
   boost::asio::streambuf buffer;

   // struct Args {
   //    std::string command;
   //    std::shared_ptr<Session> session;
   // };
   // static void worker (Args args)
   // {
   //    auto answer = database.parser(args.command);
   //    args.session->write (answer);
   // }
   ThreadPool<Args,void(Args)> otherThread;


public:
   Session (boost::asio::ip::tcp::socket socket) 
      : socket (std::move(socket))
      , otherThread(database_worker)
   { }


   void read_line()
   {
      auto self(this->shared_from_this());
      boost::asio::async_read_until (
         socket,
         buffer,
         '\n',
         [this,self] (boost::system::error_code error, std::size_t recieved) {
            if (not error) {
               boost::asio::streambuf::const_buffers_type constBuffer = buffer.data();
               std::string str (
                  boost::asio::buffers_begin(constBuffer),
                  boost::asio::buffers_begin(constBuffer) + recieved - 1
               );
               buffer.consume(recieved);
               otherThread.addMessage ({str, self});
               read_line();
            } else {
               std::cerr << error << std::endl;
            }
         }
      );
   }


   void write (const std::string& str)
   {
      auto self(this->shared_from_this());
      boost::asio::async_write (
         socket,
         boost::asio::buffer (str, str.size()),
         [this,self] (boost::system::error_code error, std::size_t) {
            if (error)
               std::cerr << error << std::endl;
         }
      );
   }

};



#include "database_worker.h"