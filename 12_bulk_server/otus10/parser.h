#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>
#include <algorithm>
#include <tuple>
#include "threadpool.h"


/// интерфейс подписчика на вывод результата работы парсера
class Exporter {
public:
   virtual void out (const std::vector<std::string>&  buffer, std::time_t time = 0) = 0;
   // virtual bool isException() = 0;
};



/// преобразует входной поток (stream) команд построчно
/// вывод средствами Exporter блоками по n 
/// maxLineSize - максимальный размер одной команды
class Parser {
public:
   /// метрики
   std::size_t lines    {0};
   std::size_t commands {0};
   std::size_t blocks   {0};

   auto metrics()
   {
      std::stringstream ss;
      ss << "main поток - "
         << lines << " строк, "
         << commands << " команд, "
         << blocks << " блок"
         << std::endl;
      return ss.str();
   }


   Parser (size_t n, size_t maxBulk = 255, size_t maxLineSize = 255)
      : n           {n},
        maxLineSize {maxLineSize},
        maxBulk     {maxBulk},
        brackets    {0},
        time        {0}
   { }


   void setN (size_t n) { this->n = n; }

   

   /// логика работы парсера
   void parse (const std::string& line)
   {
      lines++;
      if (line.size() > maxLineSize)
         throw std::length_error ("Длина команды не может превышать " + std::to_string(maxLineSize));
      if (not brackets) {
         if ( not (line == "{" or line == "}") ) {
            save_to_buffer(line);
            if (buffer.size() == n) {
               out();
            }
         } else if (line == "{") {
            brackets++;
            out();
         } else {
            buffer.clear();
         }
      } else {
         if (line == "{") {
            brackets++;
         } else if (line == "}") {
            if (--brackets == 0)
               out();
         } else {
            save_to_buffer(line);
         }
      }
      if (buffer.size() > maxBulk)
         buffer.clear();
   }
   void parse (std::istream& stream)
   {
      std::string line;
      while (std::getline (stream, line)) {
         parse (line);
      }
   }

   bool isBrackets() { return brackets; }


   /// конец файла
   void endOfFile()
   {
      if (!brackets and buffer.size())
         out();
      brackets = 0;
      buffer.clear();
   }


   /// добавляет в список подписчиков на вывод
   void subscribe(const std::shared_ptr<Exporter>& exporter) {
      exporters.push_back(exporter);
   }


   /// удаляет из списка подписчиков на вывод
   void unsubscribe(const std::shared_ptr<Exporter>& exporter) {
      std::remove_if ( exporters.begin(), exporters.end(),
         [&](const auto& element){ return element.lock() == exporter; }
      );
   }


   /// создаёт shared_ptr с подпиской на parser
   template<class Exporter>
   auto create_shared_and_subscribe()
   {
      auto ptr = std::make_shared<Exporter>();
      this->subscribe(ptr);
      return ptr; 
   }
   template<class Exporter>
   auto create_shared_and_subscribe (std::size_t threadQty)
   {
      auto ptr = std::make_shared<Exporter>(threadQty);
      this->subscribe(ptr);
      return ptr; 
   }


   std::size_t getExpsQty() { return exporters.size(); }


private:
   size_t n;
   const size_t maxLineSize;
   const size_t maxBulk;
   int brackets;
   std::time_t time;
   std::vector<std::string> buffer;
   std::vector<std::weak_ptr<Exporter>> exporters;


   /// для запоминания времени первой команды
   void save_to_buffer (const std::string& str)
   {
      if (buffer.empty())
         time = std::time(nullptr);
      buffer.push_back (str);
   }
   
  
   /// проходит по подписчикам
   /// если подписчик удалён, то удаляем и тут
   void out() {
      auto it = std::begin(exporters);
      while (it != std::end(exporters)) {
         auto ptr = it->lock();
         if (ptr) {
            ptr->out (buffer, time);
            ++it;
         } else {
            it = exporters.erase(it);
         }
      }
      // метрики
      blocks++;
      commands += buffer.size();
      buffer.clear();
   }
};



inline std::ostream& operator<< (std::ostream& stream, const std::vector<std::string>& buffer)
{
   if (buffer.size() != 0) {
      auto it = std::cbegin(buffer);
      stream << "bulk: " << *it;
      for (it = std::next(it); it != std::cend(buffer); ++it)
         stream << ", " << *it;
      stream << std::endl;
   }
   return stream;
}



/// класс вывода результатов работы парсера в консоль
struct ConsoleExporter : public Exporter {
   void out (const std::vector<std::string>& buffer, std::time_t time) override {
      std::cout << buffer;
   }
};



/// класс вывода результатов работы парсера в файл
struct FileExporter : public Exporter, public std::fstream {
   void out (const std::vector<std::string>& buffer, std::time_t time) override {
      name = "bulk" + std::to_string(time) + ".log";
      this->open (name, std::ios_base::app);
      *this << buffer;
      this->close();
   }

   std::string name;
};



/// класс вывода результатов работы парсера в stringstream
struct StringExporter : public Exporter, public std::stringstream {
   void out (const std::vector<std::string>& buffer, std::time_t time) override {
      *this << buffer;
   }
};

