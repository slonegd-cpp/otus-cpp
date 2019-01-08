#pragma once

#include <mutex>
#include <shared_mutex>

struct DataBase {
   using Table = std::map<int,std::string>;
   enum class TableName { A, B, error };
   enum class Command { insert, truncate, intersection, difference, error };

   Table A_;
   Table B_;

   mutable std::shared_timed_mutex mtx;

   std::string insert       (TableName,int,std::string);
   std::string truncate     (TableName);
   std::string intersection ();
   std::string difference   ();

   std::string parser       (const std::string&);
} database;






std::string DataBase::insert (TableName table, int id, std::string name)
{
   if (table == TableName::error)
      return "ERR Table Name Error\n";
   auto& table_ = table == TableName::A ? A_ :  B_;
   std::shared_lock<std::shared_timed_mutex> lock(mtx);
   if (table_.find (id) == std::cend(table_)) {
      table_[id] = name;
      return "OK\n";
   } else {
      return std::string {"ERR duplicate " + std::to_string(id) + '\n'};
   }
}


std::string DataBase::truncate (TableName table)
{
   if (table == TableName::error)
      return "ERR Table Name Error\n";
   auto& table_ = table == TableName::A ? A_ :  B_;
   std::unique_lock<std::shared_timed_mutex> lock(mtx);
   table_.clear();
   return "OK\n";
}


std::string DataBase::intersection()
{
   std::string res;
   std::shared_lock<std::shared_timed_mutex> lock(mtx);
   auto itA = std::begin(A_);
   auto itB = std::begin(B_);
   while (itA != std::end(A_) and itB != std::end(B_)) {
      if (itA->first < itB->first) {
         ++itA;
      } else if (itA->first > itB->first) {
         ++itB;
      } else {
         res += std::to_string(itA->first)
             + ',' + itA->second
             + ',' + itB->second
             + '\n';
         ++itA;
         ++itB;
      }
   }
   return res + "OK\n";
}


std::string DataBase::difference()
{
   std::string res;
   std::shared_lock<std::shared_timed_mutex> lock(mtx);
   auto itA = std::begin(A_);
   auto itB = std::begin(B_);
   while (itA != std::end(A_) and itB != std::end(B_)) {
      if (itA->first < itB->first) {
         res += std::to_string(itA->first)
             + ',' + itA->second
             + ','
             + '\n';
         ++itA;
      } else if (itA->first > itB->first) {
         res += std::to_string(itB->first)
             + ',' 
             + ',' + itB->second
             + '\n';
         ++itB;
      } else {
         ++itA;
         ++itB;
      }
   }
   while (itA != std::end(A_)) {
      res += std::to_string(itA->first)
            + ',' + itA->second
            + ','
            + '\n';
      ++itA;      
   }
   while (itB != std::end(B_)) {
      res += std::to_string(itB->first)
            + ',' 
            + ',' + itB->second
            + '\n';
      ++itB;    
   }
   return res + "OK\n";
}



std::string DataBase::parser (const std::string& in)
{
   std::stringstream stream {in};
   std::string str;

   stream >> str;
   Command command =
      str == "INSERT"               ? Command::insert       :
      str == "TRUNCATE"             ? Command::truncate     :
      str == "INTERSECTION"         ? Command::intersection :
      str == "SYMMETRIC_DIFFERENCE" ? Command::difference   :
                                      Command::error        ;

   switch (command) {
      case Command::error        : return "ERR Command Error\n";
      case Command::intersection : return intersection();
      case Command::difference   : return difference();
   }

   stream >> str;
   TableName tablename = str ==  "A" ? TableName::A :
                         str ==  "B" ? TableName::B :
                                       TableName::error;
   if (tablename == TableName::error)
      return "ERR Table Name Error\n";
   else if (command == Command::truncate)
      return truncate (tablename);

   stream >> str;
   if ( not std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) { return std::isdigit(c); }) )
      return "ERR ID Error\n";
   auto id = std::stoi(str);

   stream >> str;
   return insert (tablename, id, str);
}