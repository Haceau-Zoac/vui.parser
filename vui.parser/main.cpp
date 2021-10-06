#include <iostream>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::fparser("test.vui");
  int iout;
  if (obj.get("item1", iout)) std::cout << iout << '\n';
  double dout;
  if (obj.get("item2", dout)) std::cout << dout << '\n';
  std::string sout;
  if (obj.get("item4", sout)) std::cout << sout << '\n';
  bool bout;
  if (obj.get("item3", bout)) std::cout << bout << '\n';
}