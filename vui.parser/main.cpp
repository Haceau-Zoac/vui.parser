#include <iostream>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::wfparser(L"test.vui", L"World");
  int i;
  obj.get(L"d2", i);
  std::cout << i;
}