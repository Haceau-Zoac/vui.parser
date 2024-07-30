# vui.parser

为 [VertexUI](https://github.com/EnderMo/VertexUI) 写的一个简陋的解析器。

详细文档参见 [vui.parser](https://haceau-zoac.github.io/vui.parser/)。

## 目录
- [下载](#下载)
- [更新](#更新)
- [vui 语法](#vui-语法)

## 下载

1. 下载 [vui.parser.basic.hpp](https://github.com/Haceau-Zoac/vui.parser/blob/main/vui.parser/vui.parser.basic.hpp) 到 include 目录。
2. 下载 [vui.parser.hpp](https://github.com/Haceau-Zoac/vui.parser/blob/main/vui.parser/vui.parser.hpp) 到 include 目录。
3. 测试

test.vui

```
Object {
  QAQ(12),
  az("{()}"),
  ch(char),
  str(\"hello,\"),
  astr()
}
```

test.cpp

```cpp
#include <vui.parser.h>
using namespace vui::parser;

int main() {
  auto obj = wfparser(L"test1.vui");
  std::cout << "Parse " << (obj.parse() ? "succeeded" : "failed") << ".\n";
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
```

## 更新

[下载](#下载) 直接覆盖旧版文件。

## vui 语法

*对象名* : *同名对象标识符* { *数据名*(*数据*), *...* } *...*

例：
```
hello {
  world("hello, world!")
}
```
```
foo:bar {
  baz(123)
}
foo:cho {
  vui(5.43)
}
```
```
foo1 {
  u(123),
  d(5.43),
  i(-99),
  s("try")
}
```