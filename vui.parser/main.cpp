#include <iostream>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::parser("abcdefg{item1(123),item2(23.4),item3(qwq)}");
  int iout;
  if (obj.get("item1", iout)) std::cout << iout << '\n';
  double dout;
  if (obj.get("item2", dout)) std::cout << dout << '\n';
  std::string sout;
  if (obj.get("item3", sout)) std::cout << sout << '\n';
}