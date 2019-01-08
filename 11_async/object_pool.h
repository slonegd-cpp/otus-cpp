#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>


template<class T>
class ObjectPool
{
private:
   struct Object {
      std::shared_ptr<T> pointer;
      bool busy;
      Object() : pointer(std::make_shared<T>()), busy(true) {}
   };
   std::vector<std::unique_ptr<Object>> pool;

public:
   std::shared_ptr<T> get()
   {
      auto it = std::find_if (std::begin(pool), std::end(pool),
         [](auto& obj) { return not obj->busy; }
      );
      if (it != std::cend(pool)) {
         (*it)->busy = true;
         return (*it)->pointer;
      }

      pool.push_back(std::make_unique<Object>());
      return pool.back()->pointer;
   }

   void put(T* object)
   {
      auto it = std::find_if (std::begin(pool), std::end(pool),
         [object](auto& obj) { return obj->pointer.get() == object; }
      );
      if (it != std::cend(pool)) {
         (*it)->busy = false;
      }
   }

   std::shared_ptr<T> isBusy(T* object)
   {
      auto it = std::find_if (std::begin(pool), std::end(pool),
         [object](auto& obj) { return obj->pointer.get() == object and obj->busy; }
      );
      if (it != std::cend(pool)) {
         return (*it)->pointer;
      }      
      return nullptr;
   }

   ~ObjectPool()
   {
      // for (auto& p : pool) {
      //    std::cout << "!" << p.get() << " " << p->pointer->parser.metrics() << std::endl;
      //    std::cout << "!" << p.get() << " " << p->pointer->file->metrics()  << std::endl;
      // }
   }
};

