下面这份文档直接对应你仓库里那 9 个头文件，按“总览 → 快速开始 → 各组件 API → 编译要求 → 许可证”组织，可以直接保存为 `README.md` 或 `docs/README.md`。

---

# __tytech_ext

[![License: LGPL-2.1](https://img.shields.io/badge/License-LGPL--2.1-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
[![Last Commit](https://img.shields.io/github/last-commit/TengYve-Technology/__tytech_ext?color=blue)](https://github.com/TengYve-Technology/__tytech_ext/commits)
[![Header-only](https://img.shields.io/badge/Header--only-yes-success)](https://github.com/TengYve-Technology/__tytech_ext)
[![C++](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)

**编译要求：**

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/cpp/17)
[![GCC 8+](https://img.shields.io/badge/GCC-8%2B-blue)](https://gcc.gnu.org/)
[![Clang 7+](https://img.shields.io/badge/Clang-7%2B-blue)](https://clang.llvm.org/)
[![MSVC 2019+](https://img.shields.io/badge/MSVC-2019%2B-blue)](https://visualstudio.microsoft.com/)

这是一个实现于 [@FJC-2013XH](https://github.com/FJC-2013XH)（Luogu: [@_Fjc_](https://www.luogu.com.cn/user/1050571)）的扩展库。

---

## 简介

`__tytech_ext` 是一个 **header-only** 的 C++17 模板库，所有组件都位于命名空间 `__tytech_tyds` 下，通过 `inline namespace` 按功能分组。包含：

| 头文件 | 组件 | 用途 |
|---|---|---|
| `__tytech_AVL.h` | `AVL<T>` | 带重复计数、排名、第 k 小查询的平衡二叉搜索树 |
| `__tytech_BST.h` | `BST<T>` | 普通二叉搜索树（已弃用，请改用 `AVL<T>`） |
| `__tytech_Database.h` | `Database<T>` | 基于路径的树形键值数据库（类似文件系统） |
| `__tytech_Dsu.h` | `Dsu` | 支持动态键的并查集 |
| `__tytech_FastIO.h` | `FastIO` | 基于缓冲区的快速输入输出 |
| `__tytech_Hash.h` | `Hash<T>` | 字符串/可转换类型的滚动哈希 |
| `__tytech_Priority_Queue.h` | `Priority_Queue<T, Comp>` | 自定义比较器的优先队列 |

所有组件均为头文件实现，无需编译链接，直接 `#include` 即可使用。

---

## 快速开始

```cpp
#include "__tytech_AVL.h"
#include "__tytech_FastIO.h"

using namespace __tytech_tyds;

int main(){
    FastIO io;
    AVL<int> t;
    t.insert(3);
    t.insert(1);
    t.insert(2);

    io.write(t.queryRank(2));        // 输出 2
    io.write(t.queryKth(1).second);  // 输出 1
    io.flush();
    return 0;
}
```

编译：

```bash
g++ -std=c++17 -O2 main.cpp -o main
```

---

## 组件文档

### 1. `AVL<T>` —— 平衡二叉搜索树

头文件：`__tytech_AVL.h`

支持重复元素（每个节点维护 `cnt`），维护子树大小 `sz` 和高度 `ht`，插入/删除后自动旋转保持平衡。

**公开接口**

| 方法 | 返回值 | 说明 |
|---|---|---|
| `void insert(T v)` | — | 插入值 `v`，重复值只增加计数 |
| `void erase(T v)` | — | 删除一个 `v`，计数减到 0 时移除节点 |
| `bool find(T x)` | `bool` | 是否存在 `x` |
| `T findMin()` | `T` | 最小值，空树返回 `T()` |
| `T findMax()` | `T` | 最大值，空树返回 `T()` |
| `std::vector<T> traverse()` | 有序序列 | 中序遍历，按 `cnt` 展开重复值 |
| `int queryRank(T v)` | `int` | `v` 的排名（从 1 开始）；不存在时返回应插入位置 |
| `std::pair<int,T> queryKth(int k)` | `{0, 值}` 或 `{-1, T()}` | 第 `k` 小（从 1 开始），越界返回 `-1` |

**示例**

```cpp
AVL<int> t;
t.insert(5); t.insert(2); t.insert(5);
auto v = t.traverse();          // {2, 5, 5}
int r = t.queryRank(5);         // 2
auto k = t.queryKth(3);         // {0, 5}
t.erase(5);                     // 删除一个 5
```

> 注意：`queryRank` 对不存在的值返回的是“若插入后的排名”，不是 `-1`。

---

### 2. `BST<T>` —— 普通二叉搜索树（已弃用）

头文件：`__tytech_BST.h`

接口与 `AVL<T>` 基本一致，但不做平衡，最坏情况退化为链。**所有公开方法均标记 `[[deprecated]]`**，编译时会提示改用 `AVL<T>`。

```cpp
BST<int> t;   // 编译警告：please use AVL<T> instead
```

---

### 3. `Database<T>` —— 路径式键值数据库

头文件：`__tytech_Database.h`

用树形结构模拟文件系统，支持目录与文件、相对/绝对路径、节点回收复用（`delnode` 空闲表）。

**公开接口**

| 方法 | 返回值 | 说明 |
|---|---|---|
| `int put(std::string path, T val)` | `1` 成功 / `-1` 路径非法 / `0` 异常 | 写入文件，父目录必须存在 |
| `T get(std::string path)` | `T` | 读取文件；失败时先尝试相对路径，仍失败输出 `Paths Not Found!` 并返回 `T()` |
| `int del(std::string path)` | `1/0/-1` | 删除文件 |
| `int mkdir(std::string path)` | `1/0/-1` | 创建目录，已存在返回 `-1` |
| `int rmdir(std::string path)` | `1/0/-1` | 递归删除目录 |
| `int cd(std::string path)` | `1/0/-1` | 切换当前目录 |
| `int ls(std::string path)` | `1/0/-1` | 以树形打印目录内容 |

**路径规则**

- 以 `/` 开头为绝对路径，否则相对当前目录。
- 末尾 `/` 表示目录，`put`/`get` 按文件处理。
- 根目录固定为 `/`，编号 `0`。

**示例**

```cpp
Database<int> db;
db.mkdir("/a");
db.put("/a/x", 42);
int v = db.get("/a/x");   // 42
db.cd("/a");
int w = db.get("x");      // 42（相对路径）
db.ls("/");               // 打印目录树
```

> 注意：`get` 失败时通过 `std::cerr` 输出错误，并用 `catch(...)` 吞掉异常，调用方无法区分“不存在”和“值为 `T()`”。

---

### 4. `Dsu` —— 动态键并查集

头文件：`__tytech_Dsu.h`

键为 `int`，首次出现时自动分配内部编号，内部用 `unordered_map` 做键到编号的映射，`vector` 自动扩容。

**公开接口**

| 方法 | 说明 |
|---|---|
| `void merge(int x, int y)` | 合并 `x`、`y` 所在集合 |
| `int find(int x)` | 返回 `x` 所在集合的代表元（内部编号） |

**示例**

```cpp
Dsu d;
d.merge(10, 20);
d.merge(20, 30);
bool same = (d.find(10) == d.find(30));   // true
```

> 注意：`find` 返回的是内部编号，不是原始键值；只支持 `int` 键。

---

### 5. `FastIO` —— 快速输入输出

头文件：`__tytech_FastIO.h`

基于 1 MiB 输入/输出缓冲区，支持整数、浮点数、`char`、`std::string`。

**公开接口**

| 方法 | 说明 |
|---|---|
| `template<typename T> T read()` | 读一个值，`T` 可为整型/浮点/`char`/`std::string` |
| `template<typename T> void write(T x, char end='\n', int num_digits=-1)` | 写一个值；浮点默认 6 位小数 |
| `void flush()` | 刷新输出缓冲区 |

**示例**

```cpp
FastIO io;
int n = io.read<int>();
double x = io.read<double>();
std::string s = io.read<std::string>();
io.write(n);
io.write(x, '\n', 2);   // 保留 2 位小数
io.write(s);
io.flush();
```

> 注意：
> - 非浮点类型传 `num_digits != -1` 会 `abort()`。
> - `read<std::string>()` 读到换行/回车/EOF 为止。
> - 程序结束前务必 `flush()`，否则缓冲区内容可能丢失。

---

### 6. `Hash<T>` —— 滚动哈希

头文件：`__tytech_Hash.h`

把 `T` 转成字符串后做多项式哈希，默认底数 `998244353`（必须为奇数，否则抛 `std::invalid_argument`）。

**模板参数**

```cpp
template<typename T, template<typename> class U = convert_String>
class Hash;
```

`U` 是转换器，默认 `convert_String<T>`；对 `std::string` 和 `const char*` 有特化。

**构造**

| 构造 | 说明 |
|---|---|
| `Hash(T x)` | 默认底数 |
| `Hash(T x, int p)` | 指定底数 `p` |
| `Hash(T x, int p, std::string (*func)(T))` | 指定底数和转换函数 |

**公开接口**

| 方法 | 说明 |
|---|---|
| `unsigned long long gethash(int l, int r) const` | 子串 `[l, r]`（1-based）的哈希，越界抛 `std::out_of_range` |
| `unsigned long long gethash() const` | 整个字符串的哈希 |
| `bool operator==(const Hash&) const` | 长度和整串哈希都相等 |

**示例**

```cpp
Hash<std::string> h("hello");
auto full = h.gethash();
auto sub  = h.gethash(2, 4);   // "ell"
```

> 注意：这是**无模数**的 `unsigned long long` 自然溢出哈希，存在碰撞风险，对抗性数据下不建议用于安全场景。

---

### 7. `Priority_Queue<T, Comp>` —— 优先队列

头文件：`__tytech_Priority_Queue.h`

二叉堆实现，默认比较器 `mz_Comp<T>` 为 `x < y`，即**默认是大根堆**（与 `std::priority_queue` 一致）。

**模板参数**

```cpp
template<typename T, template<typename> class Comp = mz_Comp>
class Priority_Queue;
```

`Comp<T>()(a, b)` 返回 `true` 表示 `a` 应排在 `b` 前面（更优先）。

**公开接口**

| 方法 | 说明 |
|---|---|
| `void push(T x)` | 入堆 |
| `void pop()` | 弹出堆顶（空堆调用未定义） |
| `T top()` | 返回堆顶（空堆调用未定义） |
| `bool empty()` | 是否为空 |

**示例**

```cpp
Priority_Queue<int> q;          // 大根堆
q.push(3); q.push(1); q.push(2);
q.top();                        // 3

// 小根堆
template<typename T> struct Greater{ bool operator()(T a, T b){ return a > b; } };
Priority_Queue<int, Greater> pq;
```

> 注意：`pop`/`top` 不检查空堆，调用前请先 `empty()`。

---

## 编译要求

- **标准**：C++17（`FastIO.h` 使用 `if constexpr` 和 `_v` 变量模板，无法在 C++14 及以下编译）
- **编译器**：
  - GCC 8+
  - Clang 7+
  - MSVC 2019+（VS 2017 15.7 起也支持 `if constexpr`，但建议 2019+）
- **平台**：`FastIO.h` 依赖 `fread`/`fwrite`/`stdin`/`stdout`，适用于 Windows / Linux / macOS

编译示例：

```bash
g++     -std=c++17 -O2 main.cpp -o main
clang++ -std=c++17 -O2 main.cpp -o main
```

MSVC：

```bat
cl /std:c++17 /O2 main.cpp
```

---

## 命名空间

所有组件位于：

```cpp
namespace __tytech_tyds{
    inline namespace __tytech_AVL{ ... }
    inline namespace __tytech_BST{ ... }
    // ...
}
```

由于是 `inline namespace`，可以直接 `using namespace __tytech_tyds;` 后使用 `AVL<int>`、`FastIO` 等短名。

---

## 限制

- `BST<T>` 已弃用。
- `Database<T>::get` 失败时只输出到 `stderr` 并返回 `T()`，无法区分“不存在”和“默认值”。
- `Hash<T>` 为无模数自然溢出哈希，不抗碰撞攻击。
- `FastIO` 对非浮点类型传 `num_digits` 会直接 `abort()`。
- `Priority_Queue` 的 `pop`/`top` 不检查空堆。
- `Dsu` 仅支持 `int` 键，`find` 返回内部编号而非原始键。

---

## 许可证

本项目采用 **LGPL-2.1** 许可证。详见 [GNU LGPL-2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)。

---

## 作者

- [@FJC-2013XH](https://github.com/FJC-2013XH)（Luogu: [@_Fjc_](https://www.luogu.com.cn/user/1050571)）
- 仓库：[TengYve-Technology/__tytech_ext](https://github.com/TengYve-Technology/__tytech_ext)