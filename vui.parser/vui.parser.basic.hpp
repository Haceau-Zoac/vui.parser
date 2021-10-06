#pragma once
#ifndef _VUI_PARSER_H_
#define _VUI_PARSET_H_

#include <any>
#include <string>
#include <map>

namespace vui::parser
{

  template <typename StreamT, typename ArgT, typename CharT>
  class basic_parser
  {
  public:

    using object_type = std::pair<std::basic_string<CharT>,
      std::map<std::basic_string<CharT>, std::basic_string<CharT>>>;

    basic_parser(ArgT const& s) noexcept
      : stream_(s)
    {
      parse();
    }

    bool get(std::basic_string<CharT> const& key,
      std::basic_string<CharT>& result) noexcept
    {
      auto iter = obj_.second.find(key);
      if (iter == obj_.second.end())
        return false;
      result = iter->second;
      return true;
    }

  protected:
    StreamT stream_;
    object_type obj_;

    void parse() noexcept
    {
      obj_.first = read_to('{');
      CharT c{};
      do
      {
        auto first = read_to('(');
        auto second = read_to(')');
        obj_.second[first] = second;
      } while ((stream_ >> c) && (c != '}'));
    }

    auto read_to(CharT end) noexcept
    {
      std::basic_string<CharT> result;
      for (CharT c{}; (stream_ >> c) && (c != end); result += c)
        ;
      return result;
    }
  };
}

#endif // _VUI_PARSET_H_