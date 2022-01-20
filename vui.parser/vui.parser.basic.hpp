#pragma once
#ifndef _VUI_PARSER_H_
#define _VUI_PARSET_H_

#include <cctype>
#include <cinttypes>
#include <algorithm>
#include <any>
#include <string>
#include <map>
#include <locale>
#include <functional>
#include <iostream>

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
    basic_parser(ArgT const& s, string_type const& region) noexcept
      : stream_(s)
      , region_(region)
    {
      parse();
    }

    string_type root() noexcept { return obj_.first; }

    void set_region(string_type const& region) noexcept { region_ = region; }
    string_type const& region() const noexcept { return region_; }
    string_type& region() { return region_; }

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
    string_type region_;

    bool parse() noexcept
    {
      CharT c{};
      string_type obj;
      while ((stream_ >> c) && c != EOF)
      {
        switch (c)
        {
        case '#': return parse_preprocessor();
        default: return parse_object(c);
        }
      }
      return false;
    }

    bool parse_preprocessor() noexcept
    {
      if (region_.empty()) return true;

      CharT c{};
      bool is_region = false;
      while (!is_region)
      {
        if (!(stream_ >> c) || c != '#') return false;
        bool conti = false;
        for (auto const& reg : region_)
          if (!(stream_ >> c) || c != reg)
          {
            skip_to('#');
            for (std::size_t i = 0; i < 3; ++i)
              if (!(stream_ >> c) || c != '#') return false;
            conti = true;
            break;
          }
        if (conti) continue;
        is_region = true;
      }
      c = skip_whitespace();
      while (c != '#' && c != EOF)
      {
        parse_object(c);
        stream_ >> c;
      }
      if (c == EOF) return false;
      if (!(stream_ >> c) || c != '#') return false;
      if (!(stream_ >> c) || c != '#') return false;
      return true;
    }

    bool parse_object(CharT c) noexcept
    {
      string_type obj{ c };
      if (!read_to('{', obj)) return false;
      obj_.first = obj;
      do
      {
        string_type first;
        if (!read_to('(', first)) return false;
        std::any second;
        if (!read_value(second)) return false;
        obj_.second[first] = second;
      } while ((c = skip_whitespace()) != '}');
      return true;
    }

    CharT skip_whitespace() noexcept
    {
      CharT c{};
      while ((stream_ >> c) && isspace(c));
      return c;
    }

    void skip_to(CharT end) noexcept
    {
      CharT c{};
      while ((stream_ >> c) && c != end);
    }

    bool read_to(CharT end, string_type& out) noexcept
    {
      CharT c{ skip_whitespace() };
      while ((c != end) && (c != EOF))
      {
        out += c;
        stream_ >> c;
      }
      return c != EOF;
    }

    bool read_value(std::any& out) noexcept
    {
      bool is_integer = true, is_decimal = true;

      CharT c{ skip_whitespace()};
      string_type s;
      bool is_negative{ false };
      while ((c != ')') && (c != EOF))
      {
        if (!isdigit(c))
        {
          if (c == '"')
          {
            if (s.back() != '\\') 
            {
              is_integer = is_decimal = false;
              if (!read_string(s)) return false;
            }
            else 
              s.back() = '"';
            c = '\0';
          }
          else if (!is_negative && c == '-') is_negative = true;
          else
          {
            is_integer = false;
            if (c != '.') is_decimal = false;
          }
        }
        s += c;
        stream_ >> c;
      }
      if (c == EOF) return false;
      if (is_integer) out = std::stoi(s);
      else if (is_decimal) out = std::stod(s);
      else if (s.size() >= 4 && s[0] == 't' && s[1] == 'r' && s[2] == 'u' && s[3] == 'e') out = true;
      else if (s.size() >= 5 && s[0] == 'f' && s[1] == 'a' && s[2] == 'l' && s[3] == 's' && s[4] == 'e') out = false;
      else out = s;
      return true;
    }

    bool read_string(string_type& out) noexcept
    {
      CharT c{ };
      stream_ >> c;
      for (; (c != '"' || out.back() == '\\') && c != EOF; stream_ >> c)
      {
        if (c == '"') out.back() = '"';
        else out += c;
      }
      return c != EOF;
    }
  };
}

#endif // _VUI_PARSET_H_
