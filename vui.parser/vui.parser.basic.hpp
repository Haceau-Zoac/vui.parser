#pragma once
#ifndef _VUI_PARSER_H_
#define _VUI_PARSET_H_

#include <cctype>
#include <cinttypes>
#include <any>
#include <string>
#include <map>

namespace vui::parser
{

  template <typename StreamT, typename ArgT, typename CharT>
  class basic_parser
  {
  public:

    using string_type = std::basic_string<CharT>;
    using object_type = std::pair<string_type, std::map<string_type, std::any>>;

    basic_parser(ArgT const& s) noexcept
      : stream_(s)
    {
      parse();
    }

    bool root(string_type& result) noexcept
    {
      
    }

    template <typename T = string_type>
    bool get(string_type const& key, T& result) noexcept
    {
      auto iter = obj_.second.find(key);
      if (iter == obj_.second.end())
        return false;
      result = std::any_cast<T>(iter->second);
      return true;
    }

  protected:
    StreamT stream_;
    object_type obj_;

    bool parse() noexcept
    {
      std::string obj;
      if (!read_to('{', obj)) return false;
      obj_.first = obj;
      CharT c{};
      do
      {
        string_type first;
        if (!read_to('(', first)) return false;
        std::any second;
        if (!read_value(second)) return false;
        obj_.second[first] = second;
      } while ((stream_ >> c) && (c != '}'));
      return true;
    }

    bool read_to(CharT end, string_type& out) noexcept
    {
      CharT c{};
      while ((stream_ >> c) && (c != end) && (c != EOF))
        out += c;
      return c != EOF;
    }

    bool read_value(std::any& out) noexcept
    {
      bool is_integer = true, is_decimal = true;

      CharT c{};
      stream_ >> c;
      string_type s;
      while ((c != ')') && (c != EOF))
      {
        s += c;
        if (!isdigit(c))
        {
          is_integer = false;
          if (c != '.') is_decimal = false;
        }
        stream_ >> c;
      }
      if (c == EOF) return false;
      if (is_integer) out = std::stoi(s);
      else if (is_decimal) out = std::stod(s);
      else out = s;
      return true;
    }
  };
}

#endif // _VUI_PARSET_H_