#include <iostream>
#include <string>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::wfparser(L"test.vui");
  int i;
  obj.get(L"QAQ", i);
  std::cout << "QAQ: " << i << '\n';
  std::wstring az;
  obj.get(L"az", az);
  std::wcout << L"az: " << az << '\n';
  obj.get(L"ch", az);
  std::wcout << L"ch: " << az << '\n';
  obj.get(L"str", az);
  std::wcout << L"str: " << az << '\n';
  obj.get(L"astr", az);
  std::wcout << L"astr: " << az << '\n';
}
