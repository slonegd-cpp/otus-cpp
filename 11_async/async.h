#pragma once

#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include "parser.h"
#include "threadsubs.h"
#include "object_pool.h"

namespace async {

using handle_t = void *;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);



struct Handle {
   std::string buffer;
   std::string line;
   Parser parser;
   std::shared_ptr<ThreadConsoleExporter> console;
   std::shared_ptr<ThreadFileExporter> file;
   std::mutex parser_mtx;
   std::mutex pool_mtx;
   Handle(std::size_t bulk = 1) : parser (bulk)
   {
      console = parser.create_shared_and_subscribe<ThreadConsoleExporter>();
      file    = parser.create_shared_and_subscribe<ThreadFileExporter>(2);
   }

   static std::unique_ptr<ObjectPool<Handle>> pool;
   

   ~Handle()
   {
      parser_mtx.lock();
      parser.endOfFile();
      file->pool->join();
      console->pool->join();
      parser_mtx.unlock();
   }
};



}
