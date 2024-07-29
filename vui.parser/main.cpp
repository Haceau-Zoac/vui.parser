#include <iostream>
#include <string>
#include "vui.parser.hpp"
using namespace vui::parser;

int main()
{
  {
    auto obj = wfparser(L"test1.vui");
    std::cout << "Parse " << (obj.parse() ? "succeeded" : "failed") << ".\n";
    {
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
    std::cout << '\n';
    {
      int i;
      obj.get(L"QWQ", i, same_name_object<wchar_t>(L"Object", L"qwq"));
      std::wcout << L"QWQ:" << i << '\n';
    }
    std::cout << '\n';
    {
      int i;
      obj.get(L"Just", i, L"AnotherObject");
      std::cout << "AnotherObject.Just: " << i << '\n';

      std::wstring s;
      obj.get(L"Another", s, L"AnotherObject");
      std::wcout << L"AnotherObject.Another: " << s << '\n';

      double d;
      obj.get(L"Object", d, L"AnotherObject");
      std::cout << "AnotherObject.Object: " << d << '\n';
    }
    std::cout << '\n';
    {
      std::wstring out;
      obj.get(L"haha", out, L"AndThis");
      std::wcout << "AndThis.haha: " << out << '\n';

      wfparser::object_type object;
      obj.get(L"Inner", object, L"AndThis");
      object.get(L"Hello", out);
      std::wcout << L"AndThis.Inner.Hello: " << out << '\n';

      wfparser::object_type obj2;
      object.get(L"abc", obj2);
      obj2.get(L"def", out);
      std::wcout << L"AndThis.Inner.abc.def: " << out << '\n';

      int i;
      obj2.get(L"hi", i);
      std::cout << "AndThis.Inner.abc.hi: " << i << '\n';
    }
  }
  std::cout << "\n\n\n";
  {
    auto obj = vui::parser::wfparser(L"test2.vui");
    std::cout << "Parse " << (obj.parse() ? "succeeded" : "failed") << ".\n";
  }
  std::cout << "\n\n";
  {
    auto parser = vui::parser::wfparser(L"test3.vui");
    parser.parse();
    for (auto obj : parser)
    {
      std::wcout << '\n' << obj.name() << " {";
      for (auto pair : obj)
      {
        std::wcout << "\n  " << pair.name() << L"(";
        std::wstring ws;
        if (!pair.get(ws)) std::wcout << L"!not a string!";
        else std::wcout << ws;
        std::wcout << L"),";
      }
      std::wcout << L"\b\n}";
    }
  }
}
