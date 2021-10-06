#include <iostream>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::wfparser(L"test.vui");
  std::wstring out;
  if (obj.get(L"aaa", out)) std::wcout << out;
}