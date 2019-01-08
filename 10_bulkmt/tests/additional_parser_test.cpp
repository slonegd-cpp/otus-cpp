#include "parser.h"
#include <sstream>
#include <fstream>


#define BOOST_TEST_MODULE additional_parser_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (empty_and_chars_command)
{
   std::stringstream in;
   Parser parser (3, in);
   auto ss = std::make_shared<StringExporter>();
   parser.subscribe (ss);

   in << "cmd1\n"
      << "\n"
      << "Chars: abcdefghijklmnopqarstuvwxyz 1234567890 !@#$%%^&*()_+{}\n";

   parser.parse();
   parser.endOfFile();

   BOOST_CHECK_EQUAL (ss->str(), 
      "bulk: cmd1, , Chars: abcdefghijklmnopqarstuvwxyz 1234567890 !@#$%%^&*()_+{}\n"
   );
}
BOOST_AUTO_TEST_CASE (too_big_string_command)
{
   std::stringstream in;
   const int maxLineSize = 5;
   Parser parser (3, in, maxLineSize);

   in << "More then maxLineSize";

   BOOST_CHECK_THROW (parser.parse(), std::length_error);
}

BOOST_AUTO_TEST_CASE (ignore_brackets_in_command)
{
   std::stringstream in;
   Parser parser (3, in);
   auto ss = std::make_shared<StringExporter>();
   parser.subscribe (ss);

   in << "cmd1{\n"
      << "}cmd2\n"
      << "{cmd3\n"
      << "{}cmd4\n"
      << "cmd{5\n";

   parser.parse();
   parser.endOfFile();

   BOOST_CHECK_EQUAL (ss->str(), 
      "bulk: cmd1{, }cmd2, {cmd3\n"
      "bulk: {}cmd4, cmd{5\n"
   );
}

BOOST_AUTO_TEST_CASE (empty_brackets_command)
{
   std::stringstream in;
   Parser parser (3, in);
   auto ss = std::make_shared<StringExporter>();
   parser.subscribe (ss);

   in << "cmd1\n"
      << "{\n"
      << "}\n"
      << "cmd4\n"
      << "cmd5\n";

   parser.parse();
   parser.endOfFile();

   BOOST_CHECK_EQUAL (ss->str(),
      "bulk: cmd1\n"
      "bulk: cmd4, cmd5\n"
   );
}

BOOST_AUTO_TEST_SUITE_END()

