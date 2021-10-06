#pragma once
#ifndef _VUI_PARSER_HPP_
#define _VUI_PARSER_HPP_

#include "vui.parser.basic.hpp"
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

namespace vui::parser
{
  using parser = basic_parser<std::stringstream, std::string, char>;
  using wparser = basic_parser<std::wstringstream, std::wstring, wchar_t>;

  using fparser = basic_parser<std::fstream, std::string, char>;
  using wfparser = basic_parser<std::wfstream, std::wstring, wchar_t>;
}

#endif // _VUI_PARSER_HPP_