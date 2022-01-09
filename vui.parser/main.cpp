#include <iostream>
#include <string>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::wfparser(L"test.vui", L"World");
  int i;
  obj.get(L"d2", i);
  std::cout << i;
  std::wstring s;
  obj.get(L"d3", s);
  std::wcout << s;
}