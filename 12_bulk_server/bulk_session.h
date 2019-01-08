#pragma once

#include <vector>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "parser.h"


class BulkSession : public std::enable_shared_from_this<BulkSession>
{
public:
   BulkSession (
      boost::asio::ip::tcp::socket socket,
      Parser& parser,
      size_t bulk,
      std::size_t maxLineSize = 255
   )
      : socket      (std::move(socket)),
        commonParser(parser),
        innerParser (bulk),
        maxLineSize (maxLineSize),
        buffer      (maxLineSize)
   {

   }

   auto subscribe (const std::vector<std::shared_ptr<Exporter>>& exporters)
   {
      for (const auto& e : exporters)
         innerParser.subscribe (e);
      return this->shared_from_this();
   }


   auto subscribe (const std::shared_ptr<Exporter>& exporter)
   {
      innerParser.subscribe (exporter);
      return this->shared_from_this();
   }


   auto read_line() -> decltype(this->shared_from_this())
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
               if (innerParser.isBrackets() or str == "{" )
                  innerParser.parse (str);
               else
                  commonParser.parse (str);
               read_line();
            } else {
               std::cerr << error << std::endl;
            }
         }
      );
      return self;
   }


   std::size_t subscribedQty() { return innerParser.getExpsQty(); }


private:
   boost::asio::ip::tcp::socket socket;
   boost::asio::streambuf buffer;
   Parser& commonParser;
   Parser  innerParser;
   const std::size_t maxLineSize;

   
};