#pragma once

#include "parser.h"
#include <map>
#include <random>
		



/// интерфейс подписчика на вывод результата работы парсера в других потоках
struct ThreadExporter : public Exporter {

   /// перечисление аргументов для функции, которая выполняется в других потоках
   enum Index { buffer = 0, time, integer, cout_mutex, metrics_mutex };
   /// тип аргументов для функции, которая выполняется в других потоках
   using Args = std::tuple<
      std::vector<std::string>,
      std::time_t,
      std::size_t,
      std::shared_ptr<std::mutex>,
      std::shared_ptr<std::mutex>
   >;

   void out (const std::vector<std::string>& buffer, std::time_t time) override {
      pool->addMessage (std::make_tuple(buffer,time,index++,cout_mtx,metrics_mtx));
   }

   void setCoutMutex (std::shared_ptr<std::mutex> cout_mtx) { this->cout_mtx = cout_mtx; }

   using ThreadType = ThreadPool<Args,void(Args)>;
   std::unique_ptr<ThreadType> pool {nullptr};
   /// для уникальности имён файлов
   size_t index {0};
   std::shared_ptr<std::mutex> cout_mtx;
   std::shared_ptr<std::mutex> metrics_mtx;
};



/// класс вывода результатов работы парсера в файл в других потоках
struct ThreadFileExporter : public ThreadExporter {
   
   static void toFile (typename ThreadExporter::Args args)
   {
      auto buffer = std::get<ThreadExporter::Index::buffer>(args);
      if (buffer.size()) {
         auto id = std::this_thread::get_id();
         auto name = std::string (
            "bulk" 
            + std::to_string(std::get<ThreadExporter::Index::time>(args))
            + '_'
            + [id](){ std::stringstream s; s << id; return s.str(); }()
            + '_'
            + std::to_string(std::get<ThreadExporter::Index::integer>(args))
            + ".log"
         );
         std::fstream file(name, std::ios_base::app);
         file << buffer;
         if (file.fail() or failTest)
            throw std::runtime_error("out of memory");

         // метрики
         auto metrics_mtx = std::get<ThreadExporter::Index::metrics_mutex>(args);
         metrics_mtx->lock();
         blocksQty[id]++;
         cmdQty[id] += buffer.size();
         metrics_mtx->unlock();

         // doSome(buffer);
      }
   }

   ThreadFileExporter (std::size_t threadQty = 1)
   {
      this->pool = std::make_unique<typename ThreadExporter::ThreadType>(toFile, threadQty);
      this->metrics_mtx = std::make_shared<std::mutex>();
   }


   /// метрики
   static std::map<std::thread::id, std::size_t> blocksQty;
   static std::map<std::thread::id, std::size_t> cmdQty;
   /// вывод метрик
   auto metrics()
   {
      std::stringstream ss;
      std::size_t i {1};
      auto it = std::cbegin(blocksQty);
      for (auto i{1}; i <= this->pool->threadQty; ++i) {
         if (it != std::cend(blocksQty)) {
            auto id = it->first;
            ss << "file" << i << " поток - "
            << blocksQty[id] << " блок, "
            << cmdQty[id] << " комманд" << std::endl;
            ++it;
         } else {
            ss << "file" << i << " поток - 0 блоков, 0 комманд" << std::endl;
         }
      }
      return ss.str();
   }

   /// для тестирования
   static bool failTest;

   /// дополнительная нагрузка
   static size_t loadCPU;
   static void doSome (std::vector<std::string>& v)
   {
      std::random_device rd;
      std::mt19937 g(rd());
      for (std::size_t i = 0; i < loadCPU; ++i)
         std::shuffle(std::begin(v), std::end(v), g);
   }
};



/// класс вывода результатов работы парсера в консоль в других потоках
struct ThreadConsoleExporter : public ThreadExporter {
   
   static void toConsole (typename ThreadExporter::Args args)
   {
      auto buffer   = std::get<ThreadExporter::Index::buffer>(args);
      auto cout_mtx = std::get<ThreadExporter::Index::cout_mutex>(args);
      if (buffer.size()) {
         if (cout_mtx) {
            cout_mtx->lock();
            std::cout << buffer;
            cout_mtx->unlock();
         } else {
            std::cout << buffer;
         }
         // метрики
         blocksQty++;
         cmdQty += buffer.size();
      }
   }

   ThreadConsoleExporter()
   {
      this->pool = std::make_unique<typename ThreadExporter::ThreadType>(toConsole, 1);
   }


   /// метрики
   static std::size_t blocksQty;
   static std::size_t cmdQty;
   /// вывод метрик
   auto metrics()
   {
      std::stringstream ss;
      ss << "log" << " поток - " 
      << blocksQty << " блок, "
      << cmdQty << " комманд" << std::endl;
      return ss.str();
   }

};

