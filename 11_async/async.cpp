#include "async.h"


namespace async {


std::unique_ptr<ObjectPool<Handle>> Handle::pool {nullptr};
static std::mutex pool_mtx;

static auto cout_mutex = std::make_shared<std::mutex>();


handle_t connect(std::size_t bulk)
{
   bool tmp;
   pool_mtx.lock();
   if (not Handle::pool)
      Handle::pool = std::make_unique<ObjectPool<Handle>>();
   auto h = Handle::pool->get();
   h->parser_mtx.lock();
   pool_mtx.unlock();
   h->parser.setN (bulk);
   h->console->setCoutMutex(cout_mutex);
   h->parser_mtx.unlock();
   return h.get();
}

void receive(handle_t handle, const char *data, std::size_t size)
{
   pool_mtx.lock();
   if (Handle::pool and handle) {
      auto h = Handle::pool->isBusy(reinterpret_cast<Handle*>(handle));
      if (h) {
         h->parser_mtx.lock();
         pool_mtx.unlock();
         std::string str {data, size};
         for (const auto& c : str) {
            if ( c != '\n' ) {
               h->line.push_back (c);
            } else {
               h->parser.parse (h->line);
               h->line.clear();
            }
         }
         h->parser_mtx.unlock();
      } else {
         pool_mtx.unlock();
      }
   } else {
      pool_mtx.unlock();
   }
   
}

void disconnect(handle_t handle)
{
   pool_mtx.lock();
   if (Handle::pool and handle) {
      auto h = reinterpret_cast<Handle*>(handle);
      h->parser_mtx.lock();
      Handle::pool->put(h);
      pool_mtx.unlock();
      h->parser.endOfFile();
      h->line.clear();
      h->file->pool->waitAll();
      h->console->pool->waitAll();
      h->parser_mtx.unlock();
   } else {
      pool_mtx.unlock();
   }
   
}

} // namespace async {
