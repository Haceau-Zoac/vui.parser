#pragma once
#ifndef VUI_PARSER_H_
#define VUI_PARSET_H_

#include <cctype>
#include <cinttypes>
#include <algorithm>
#include <any>
#include <string>
#include <unordered_map>
#include <locale>
#include <functional>
#include <iostream>
#include <optional>

namespace vui::parser
{
  template <typename StreamT, typename ArgT, typename CharT>
  class basic_parser
  {
  public:

    using string_type = std::basic_string<CharT>;
    using object_type = std::unordered_map<string_type, std::any>;

    basic_parser(ArgT const& s) noexcept
    : stream_(s)  { }
    basic_parser(ArgT const& s, string_type const& region) noexcept
      : stream_(s)
      , region_(region) { }

    void set_region(string_type const& region) noexcept { region_ = region; }
    string_type const& region() const noexcept { return region_; }
    string_type& region() { return region_; }

    template <typename T = string_type>
    bool get(string_type const& key, T& result, std::optional<string_type> const& name = std::nullopt) noexcept
    {
      if (!objs_.has_value())
      {
        parse();
      }
      auto& objs{ objs_.value() };
      object_type obj;
      if (!name.has_value())
        obj = objs.begin()->second;
      else if (!objs.count(name.value()))
        return false;
      else
        obj = objs[name.value()];
      
      if (!obj.count(key))
        return false;
      std::any value{ obj[key] };
      if (value.type() != typeid(T))
        return false;
      result = std::any_cast<T>(value);
      return true;
    }

    bool parse() noexcept
    {
      objs_ = std::unordered_map<string_type, object_type>{};
      CharT c{};
      string_type obj;
      while ((!stream_.eof()) && (stream_ >> c))
      {
        switch (c)
        {
        case '#': return parse_preprocessor();
        default: {
          if (!parse_object(c)) return false;
          while ((c = skip_whitespace()) == ',')
          {
            stream_ >> c;
            if (!parse_object(c)) return false;
          }
          return true;
        }
        }
      }
      return false;
    }


  protected:
    StreamT stream_;
    std::optional<std::unordered_map<string_type, object_type>> objs_;
    string_type region_;

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
      while (c != '#' && (!stream_.eof()))
      {
        parse_object(c);
        stream_ >> c;
      }
      if (stream_.eof()) return false;
      if (!(stream_ >> c) || c != '#') return false;
      if (!(stream_ >> c) || c != '#') return false;
      return true;
    }

    bool parse_object(CharT c) noexcept
    {
      string_type name{ c };
      if (!read_to('{', name)) return false;
      object_type obj;
      do
      {
        string_type first;
        if (!read_to('(', first)) return false;
        std::any second;
        if (!read_value(second)) return false;
        obj[first] = second;
      } while ((c = skip_whitespace()) != '}');
      objs_.value()[name] = obj;
      return true;
    }

    CharT skip_whitespace() noexcept
    {
      CharT c{};
      while ((stream_ >> c) && c <= 255 && isspace(c));
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
      while ((c != end) && (!stream_.eof()))
      {
        out += c;
        stream_ >> c;
      }
      return !stream_.eof();
    }

    bool read_value(std::any& out) noexcept
    {
      bool is_integer = true, is_decimal = true;

      CharT c{ skip_whitespace()};
      string_type s{};
      bool is_negative{ false };
      while ((c != ')') && (!stream_.eof()))
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
      if (stream_.eof()) return false;
      if (s.empty()) out = "";
      else if (is_integer) out = std::stoi(s);
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
      for (; (c != '"' || out.back() == '\\') && (!stream_.eof()); stream_ >> c)
      {
        if (c == '"') out.back() = '"';
        else out += c;
      }
      return !stream_.eof();
    }
  };
}

#endif // VUI_PARSET_H_
