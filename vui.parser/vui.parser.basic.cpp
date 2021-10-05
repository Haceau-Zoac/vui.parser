#include "vui.parser.basic.h"

#include <fstream>
#include <string>
#include <memory>
#include <functional>
#include <map>

namespace vui::parser::basic
{
  object_type parse(char const* path)
  {
    std::fstream file{ path };
    object_type result;
    result.first = read_to(file, '{');
    char c;
    do
    {
      std::string first = read_to(file, '(');
      std::string second = read_to(file, ')');
      result.second[first] = second;
    } while ((file >> c) && (c != '}'));
    return result;
  }

  static std::string read_to(std::fstream& file, char end)
  {
    std::string result;
    for (char c; (file >> c) && (c != end); result += c)
      ;
    return result;
  }
}