# Test — C++ 学习工程

一个用来练习现代 C++ 和 CMake 工程结构的最小项目。代码量不大，但把日常 C++ 项目里该有的东西都搭起来了：标准版本固定、编译告警严格、库与可执行文件分离、单元测试、跨平台构建脚本。

## 目录结构

```
.
├── CMakeLists.txt          # 顶层构建脚本
├── include/test/           # 公开头文件
│   ├── greeter.h
│   └── math_utils.h
├── src/                    # 实现与入口
│   ├── greeter.cpp
│   ├── math_utils.cpp
│   └── main.cpp
└── tests/                  # GoogleTest 单元测试
    ├── CMakeLists.txt
    ├── greeter_test.cpp
    └── math_utils_test.cpp
```

## 环境要求

- CMake 3.14 或更高
- 支持 C++17 的编译器（MSVC 2019+、GCC 8+、Clang 7+）
- 首次配置测试时需要联网：GoogleTest 通过 `FetchContent` 下载

## 构建与运行

```bash
cmake -S . -B build
cmake --build build --config Release
./build/test_app          # Linux / macOS
.\build\Release\test_app.exe   # Windows，多配置生成器在 Release 子目录下
```

运行结果输出 `Hello, world!`。

## 运行测试

```bash
ctest --test-dir build --output-on-failure
```

`gtest_discover_tests` 会把每个用例注册成独立的 CTest 测试项，失败时能看到具体是哪个 `TEST` 挂了。也可以直接跑测试程序 `unit_tests`。

关闭测试构建（不需要联网下载依赖时）：

```bash
cmake -S . -B build -DTEST_BUILD_TESTS=OFF
```

## 工程结构要点

**目标拆成两个。** `test_lib` 装所有实现（`src/` 下除 `main.cpp` 以外的文件），`test_app` 只放入口并链接 `test_lib`。测试目标同样链接 `test_lib`，这样同一份代码既被程序使用也被测试使用，避免测试和实际运行的是两份拷贝。

**标准版本锁死。** `CMAKE_CXX_STANDARD 17`、`CMAKE_CXX_STANDARD_REQUIRED ON`、`CMAKE_CXX_EXTENSIONS OFF`。最后一项关掉编译器扩展，保证写的是标准 C++，在 GCC/Clang/MSVC 之间行为一致。

**告警即错误。** MSVC 用 `/W4 /WX`，其他编译器用 `-Wall -Wextra -Wpedantic -Werror`。学习阶段这点很值得保留：很多未定义行为、隐式转换、未使用变量的问题，编译器提前就替你拦住了。

**`CONFIGURE_DEPENDS`。** 用 `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)` 收集源文件，新增 `.cpp` 时不用手改 CMakeLists，重新构建会自动纳入。

**头文件在 `include/test/`。** 公共接口单独目录，`target_include_directories(test_lib PUBLIC include)` 让 `#include "test/greeter.h"` 这种带命名空间的写法可用，也方便以后把这个目录整个发布出去。

**`compile_commands.json`。** `CMAKE_EXPORT_COMPILE_COMMANDS ON` 会生成编译数据库，clangd、VSCode C/C++ 插件、CLion 都靠它给补全和跳转。

## 代码里的两个例子

`test::greet` 演示最基本的字符串拼接和 `std::string` 传参——注意参数是 `const std::string&`，避免拷贝。

`test::math_utils.h` 里 `add` 的注释点出一个容易踩的坑：

```cpp
// Returns a + b. Signed overflow is undefined behavior, as with plain int addition.
int add(int a, int b);
```

有符号整数溢出在 C++ 里是未定义行为，不是回绕。编译器可以据此做各种优化，结果不可预测。测试里 `HandlesLimitsWithoutOverflow` 只对不溢出的边界组合做断言，就是这个原因。需要可靠的溢出行为时，应该显式检查（比如用 `__builtin_add_overflow` 或者先判断范围），而不是指望回绕。

## 可以继续练的方向

- 给 `test::greet` 加参数化测试（`TEST_P` / `INSTANTIATE_TEST_SUITE_P`）
- 把 `add` 模板化，再为 `unsigned` 版本实现良定义的溢出语义
- 加 `clang-format` 和 `clang-tidy` 配置，挂到 CMake 的 target 上
- 接 GitHub Actions，在 Windows / Linux / macOS 上分别构建和跑测试
- 用 `Benchmark`（Google Benchmark）测一下 `greet` 的字符串拼接开销，比较 `+` 与 `std::string_view` 的差异
