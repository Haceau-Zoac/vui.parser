#include <iostream>
#include "vui.parser.basic.h"

int main()
{
  auto ret = vui::parser::basic::parse("test.vui");
  std::cout << "Root: " << ret.first << std::endl;
  std::cout << "Nodes: " << std::endl;
  for (auto const& item : ret.second)
    std::cout << "\tName: " << item.first << "\n\tValue: " << item.second << "\n\n";
}