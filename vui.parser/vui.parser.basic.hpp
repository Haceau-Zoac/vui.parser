/**
 * @file vui.parser.basic.hpp
 * @author Haceau (haceau@qq.com)
 * @brief vui 解析器的通用基本类。
 * @version 0.1.0
 * @date 2022-02-20
 * @copyright Copyright (c) 2022
 * 
 */
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
  ///
  /// @class basic_parser<StreamT, ArgT, CharT> 
  /// @brief 通用的 vui 格式解析类。
  ///
  template <typename StreamT, typename CharT>
  class basic_parser
  {
  public:

    /// @brief 解析器使用的字符串类型。
    using string_type = std::basic_string<CharT>;
    /// @brief 解析器使用的对象类型。
    using object_type = std::unordered_map<string_type, std::any>;

    /// @brief 初始化器。
    /// @param s 要解析的流。
    template<typename T>
    basic_parser(T const& s) noexcept
    : stream_(s)  { }
    /// @brief 初始化器。
    /// @param s 要解析的流。
    /// @param region 要解析的 region。
    template<typename T>
    basic_parser(T const& s, string_type const& region) noexcept
      : stream_(s)
      , region_(region) { }

    /// @brief 设置 region。
    /// @param region 要解析的 region。
    void set_region(string_type const& region) noexcept { region_ = region; }
    /// @brief 获取 region。
    /// @return 要解析的的 region。
    string_type const& region() const noexcept { return region_; }

    /// @brief 获取数据。
    /// @param key 要获取的数据的名称。
    /// @param result [返回] 获取到的数据。若获取失败，`result` 中的内容不改变。
    /// @param name [可选] 要获取数据的对象的名称。默认为解析的第一个对象。
    /// @return 成功返回 `true`，失败返回 `false`。
    ///
    /// 若在 `get` 前未进行过 `parse`，将会自动执行一次 `parse`。
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

    /// @brief 解析流中的数据。
    /// @return 成功返回 `true`，失败返回 `false`。
    /// 
    /// 只能 `parse` 一次，多次 `parse` 将返回 `false`。
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
