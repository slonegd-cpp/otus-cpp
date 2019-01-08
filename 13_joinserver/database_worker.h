#pragma once


void intersection (std::shared_ptr<Session> session)
{
   std::string res;
   std::shared_lock<std::shared_timed_mutex> lock(database.mtx);
   auto itA = std::begin(database.A_);
   auto itB = std::begin(database.B_);
   while (itA != std::end(database.A_) and itB != std::end(database.B_)) {
      if (itA->first < itB->first) {
         ++itA;
      } else if (itA->first > itB->first) {
         ++itB;
      } else {
         session->write (
             std::to_string(itA->first)
             + ',' + itA->second
             + ',' + itB->second
             + '\n'
         );
         ++itA;
         ++itB;
      }
   }
   session->write ("OK\n");
}

void difference (std::shared_ptr<Session> session)
{
   std::string res;
   std::shared_lock<std::shared_timed_mutex> lock(database.mtx);
   auto itA = std::begin(database.A_);
   auto itB = std::begin(database.B_);
   while (itA != std::end(database.A_) and itB != std::end(database.B_)) {
      if (itA->first < itB->first) {
         session->write (
               std::to_string(itA->first)
             + ',' + itA->second
             + ','
             + '\n'
         );
         ++itA;
      } else if (itA->first > itB->first) {
         session->write (
               std::to_string(itB->first)
             + ',' 
             + ',' + itB->second
             + '\n'
         );
         ++itB;
      } else {
         ++itA;
         ++itB;
      }
   }
   while (itA != std::end(database.A_)) {
      session->write (
              std::to_string(itA->first)
            + ',' + itA->second
            + ','
            + '\n'
      );
      ++itA;      
   }
   while (itB != std::end(database.B_)) {
      session->write (
              std::to_string(itB->first)
            + ',' 
            + ',' + itB->second
            + '\n'
      );
      ++itB;    
   }
   session->write ("OK\n");
}


void database_worker (Args args)
{
   using Command   = DataBase::Command;
   using TableName = DataBase::TableName;

   std::stringstream stream {args.command};
   std::string str;

   stream >> str;
   Command command =
      str == "INSERT"               ? Command::insert       :
      str == "TRUNCATE"             ? Command::truncate     :
      str == "INTERSECTION"         ? Command::intersection :
      str == "SYMMETRIC_DIFFERENCE" ? Command::difference   :
                                      Command::error        ;

   switch (command) {
      case Command::error        : args.session->write("ERR Command Error\n"); return;
      case Command::intersection : intersection(args.session); return;
      case Command::difference   : difference(args.session); return;
   }

   stream >> str;
   TableName tablename = str ==  "A" ? TableName::A :
                         str ==  "B" ? TableName::B :
                                       TableName::error;
   if (tablename == TableName::error) {
      args.session->write("ERR Table Name Error\n");
      return;
   } else if (command == Command::truncate) {
      args.session->write(database.truncate (tablename));
      return;
   }

   stream >> str;
   if ( not std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) { return std::isdigit(c); }) ) {
      args.session->write("ERR ID Error\n");
      return;
   }
      
   auto id = std::stoi(str);

   stream >> str;
   args.session->write(database.insert (tablename, id, str));
}

