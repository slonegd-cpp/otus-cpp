#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>
#include <algorithm>


/// интерфейс подписчика на вывод результата работы парсера
class Exporter {
public:
   virtual void out (const std::vector<std::string>&  buffer, std::time_t time = 0) = 0;
};



/// преобразует входной поток (stream) команд построчно
/// вывод средствами Exporter блоками по n 
/// maxLineSize - максимальный размер одной команды
class Parser {
public:
   Parser (size_t n, std::istream& stream, size_t maxLineSize = 255)
      : n           {n},
        maxLineSize {maxLineSize},
        brackets    {0},
        time        {0},
        stream      {stream}
   { }


   /// логика работы парсера
   void operator ()()
   {
      std::string line;
      while (std::getline (stream, line)) {
         if (line.size() > maxLineSize)
            throw std::length_error ("Длина команды не может превышать " + std::to_string(maxLineSize));
         if (!brackets) {
            if (line != "{") {
               save_to_buffer(line);
               if (buffer.size() == n)
                  out();
            } else {
               brackets++;
               out();
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
      } // while (std::getline (stream, line)) {

      if (!brackets)
         out();
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


private:
   const size_t n;
   const size_t maxLineSize;
   int brackets;
   std::time_t time;
   std::istream& stream;
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
      buffer.clear();
   }
};



static std::ostream& operator<< (std::ostream& stream, const std::vector<std::string>& buffer)
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

