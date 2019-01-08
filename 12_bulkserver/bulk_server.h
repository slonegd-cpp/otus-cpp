#pragma once

#include "bulk_session.h"


class BulkServer : public std::enable_shared_from_this<BulkServer>
{
public:
   BulkServer (
      boost::asio::io_service& service,
      const boost::asio::ip::tcp::endpoint& endpoint,
      std::size_t bulk
   ) 
      : acceptor (service, endpoint),
        socket   (service),
        parser   (bulk),
        bulk     (bulk)
   {

   }


   auto subscribe (const std::shared_ptr<Exporter>& exporter)
   {
      exporters.push_back (exporter);
      parser.subscribe (exporter);
      return this->shared_from_this();
   }


   auto start() -> decltype(this->shared_from_this())
   {
      acceptor.async_accept (
         socket,
         [this] (boost::system::error_code error) {
            if (not error)
               std::make_shared<BulkSession>(std::move(socket), parser, bulk)
                  -> subscribe (exporters)
                  -> read_line();

            start();
         }
      );
      return this->shared_from_this();
   }


private:
   boost::asio::ip::tcp::acceptor acceptor;
   boost::asio::ip::tcp::socket   socket;
   std::vector<std::shared_ptr<Exporter>> exporters;
   Parser parser;
   std::size_t bulk;

};