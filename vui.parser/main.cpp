#include <iostream>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::fparser("test.vui", "World");
  int i;
  obj.get("d2", i);
  std::cout << i;
}