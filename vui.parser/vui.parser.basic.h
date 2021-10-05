#pragma once
#ifndef _VUI_PARSER_H_
#define _VUI_PARSET_H_

#include <any>
#include <string>
#include <fstream>
#include <functional>
#include <map>
#include <string>

namespace vui::parser::basic
{
  using object_type = std::pair<std::string, std::map<std::string, std::string>>;

  object_type parse(char const* path);
  static std::string read_to(std::fstream& file, char end);
}

#endif // _VUI_PARSET_H_