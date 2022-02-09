#include <iostream>
#include <string>
#include "vui.parser.hpp"

int main()
{
  auto obj = vui::parser::wfparser(L"test.vui");
  std::wstring az;
  if (obj.get(L"QAQ", az)) {
    std::wcout << L"QAQ: " << az << '\n';
  } else {
    std::cout << "QAQ: Get failed.\n";
  }
  if (obj.get(L"az", az)) {
    std::wcout << L"az: " << az << '\n';
  } else {
    std::cout << "az: Get failed.\n";
  }
  obj.get(L"ch", az);
  std::wcout << L"ch: " << az << '\n';
  obj.get(L"str", az);
  std::wcout << L"str: " << az << '\n';
  if (obj.get(L"astr", az)) {
    std::wcout << L"astr: " << az << '\n';
  } else {
    std::cout << "astr: Get failed.\n";
  }
}
