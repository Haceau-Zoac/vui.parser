#include <iostream>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::wparser(L"qwq{awa(owo)}");
  std::wstring ret;
  obj.get(L"awa", ret);
  std::wcout << ret;
}