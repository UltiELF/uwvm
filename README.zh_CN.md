<div style="text-align:center">
    <img src="documents/images/logo_256x256.png" , alt="logo" />
    <h1>Ultimate WebAssembly Virtual Machine</h1>
    <a href="https://github.com/UltiELF/uwvm/actions?query=workflow%3Auwvm-build">
      <img src="https://img.shields.io/github/actions/workflow/status/UltiELF/uwvm/uwvm.yml?branch=dev&logo=linux" alt="github-ci" />
    </a>
    <a href="LICENSE.md">
        <img src="https://img.shields.io/badge/License-Apache%202.0-green.svg" , alt="License" />
    </a>
    <a href="https://zh.cppreference.com">
        <img src="https://img.shields.io/badge/language-c++23-blue.svg" ,alt="cppreference" />
    </a>
    <a
        href="http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=VwOd_SiQ31UIiX_QtI047ngYYgkzvvlB&authKey=HJecYKgB1HQCtIplBkNjeSxlat8OgNXtk47QURCS6y2c7dAifwHaKZaURIci6yE3&noverify=0&group_code=137136029">
        <img src="https://img.shields.io/badge/chat-on%20QQ-red.svg" , alt="QQ" />
    </a>
    <a
        href="https://discord.gg/xkvGy79e">
        <img src="https://img.shields.io/badge/chat-on%20Discord-7289da.svg" , alt="Discord" />
    </a>
</div>

|[English](README.md)|

## 联系我们

- [Discord](https://discord.gg/xkvGy79e)
- QQ: [137136029](http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=VwOd_SiQ31UIiX_QtI047ngYYgkzvvlB&authKey=HJecYKgB1HQCtIplBkNjeSxlat8OgNXtk47QURCS6y2c7dAifwHaKZaURIci6yE3&noverify=0&group_code=137136029)
- 仓库: [Gitee](https://gitee.com/UltiELF/uwvm), [GitHub](https://github.com/UltiELF/uwvm)

## 介绍
Ultimate WebAssembly 虚拟机

## 命令行使用
* 获取版本信息
```bash
$ uwvm --version
```
* 获取命令列表
```bash
$ uwvm --help
```
* 运行虚拟机
```bash
$ uwvm <param1> <param2> ... --run <file> <argv1> <argv2> ...
```
* 运行模式
```bash
--mode [objdump(默认)]
```
* 加载 WASM ABI (默认自动检测)
```bash
--abi [bare|emscripten|wasip1|wasip2]
```

## 特征
### 功能扩展
| 功能                                                                                                                                               |          参数                      |
|----------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------|
| [JS BigInt to Wasm i64 integration](https://github.com/WebAssembly/JS-BigInt-integration)                                                          |  N/A                               |
| [Bulk memory operations](https://github.com/WebAssembly/bulk-memory-operations/blob/master/proposals/bulk-memory-operations/Overview.md)           |  :x:                               |
| [Extended constant expressions](https://github.com/WebAssembly/extended-const/blob/master/proposals/extended-const/Overview.md)                    |  :x:                               |
| [Garbage collection](https://github.com/WebAssembly/gc)                                                                                            |  :x:                               |
| [Multiple memories](https://github.com/WebAssembly/multi-memory/blob/master/proposals/multi-memory/Overview.md)                                    |  ```--enable-multi-memory```       |
| [Multi-value](https://github.com/WebAssembly/spec/blob/master/proposals/multi-value/Overview.md)                                                   |  ```--enable-multi-value```        |
| [Mutable globals](https://github.com/WebAssembly/mutable-global/blob/master/proposals/mutable-global/Overview.md)                                  |  ```--enable-mutable-globals```    |
| [Reference types](https://github.com/WebAssembly/reference-types/blob/master/proposals/reference-types/Overview.md)                                |  ```--enable-reference-types```    |
| [Relaxed SIMD](https://github.com/WebAssembly/relaxed-simd/tree/main/proposals/relaxed-simd)                                                       |  ```--enable-relaxed-simd```       |
| [Non-trapping float-to-int conversions](https://github.com/WebAssembly/spec/blob/master/proposals/nontrapping-float-to-int-conversion/Overview.md) |  :x:                               |
| [Sign-extension operations](https://github.com/WebAssembly/spec/blob/master/proposals/sign-extension-ops/Overview.md)                              |  :x:                               |
| [Fixed-width SIMD](https://github.com/WebAssembly/simd/blob/master/proposals/simd/SIMD.md)                                                         |  ```--enable-fixed-width-simd```   |
| [Extended constant expressions](https://github.com/WebAssembly/extended-const/blob/master/proposals/extended-const/Overview.md)                    |  :x:                               |
| [Tail calls](https://github.com/WebAssembly/tail-call/blob/master/proposals/tail-call/Overview.md)                                                 |  :x:                               |
| [Threads and atomics](https://github.com/WebAssembly/threads/blob/master/proposals/threads/Overview.md)                                            |  :x:                               |
| [Exception handling](https://github.com/WebAssembly/exception-handling/blob/master/proposals/exception-handling/Exceptions.md)                     |  ```--enable-exception-handling``` |
| [JS Promise Integration](https://github.com/WebAssembly/js-promise-integration)                                                                    |  N/A                               |
| [Memory64](https://github.com/WebAssembly/memory64/blob/master/proposals/memory64/Overview.md)                                                     |  ```--enable-memory64```           |
| [Type reflection](https://github.com/WebAssembly/js-types/blob/main/proposals/js-types/Overview.md)                                                |  :x:                               |

### 多平台支持
* (x86\_64, i386, aarch64, arm)-windows-(gnu, msvc) (支持UCRT和MSVCRT)
* (x86\_64, i386, aarch64, arm, loongarch64, riscv64, mips64, powerpc64, etc.)-linux-(gnu, musl, llvm, mlibc, uclibc, avrlibc, etc.)
* (x86\_64, i386, aarch64, arm, powerpc, powerpc64, powerpc64le, powerpcspe, riscv64)-freebsd
* unknown-dragonflybsd
* unknown-netbsd
* unknown-openbsd
* unknown-bsd
* unknown-sun
* unknown-apple-darwin
* i386-msdosdjgpp
* wasm32-wasip1, wasm32-wasip2, wasm64-wasip1, wasm64-wasip2 (自举)
* etc.

## 如何构建
1. 安装 [[xmake]](https://github.com/xmake-io/xmake/)
2. 安装 [[gcc14+]](https://github.com/trcrsired/gcc-releases/releases) or [[llvm18+]](https://github.com/trcrsired/llvm-releases/releases)
3. 开始构建
```bash
$ xmake 
$ xmake install -o OutputPath 
```
4. 构建参数
```bash
$ xmake f -m [release|releasedbg|debug] -p [windows|mingw|linux|sun|msdosdjgpp|bsd|freebsd|dragonflybsd|netbsd|openbsd|macosx|iphoneos|watchos|wasm-wasip1|wasm-wasip2|cross ..] -a [x86_64|i386|arm|aarch64 ..] --cppstdlib=[default|libstdc++|libc++] ..
```
* 目前只支持 msvc 14.30+ ，gcc 14+ 以及 llvm 18+。
* 若要编译兼容 WIN10 (默认) 以下系统，请添加参数
```bash 
--min-win32-sys=[WIN10|WINBLUE|WIN8|WIN7|WS08|VISTA|WS03|WINXP|WIN2K]
```
* 对于WIN9X (i386-windows-gnu)
```bash
--min-win32-sys=[WINME|WIN98|WIN95]
```
* 使用llvm工具链
```bash 
--use-llvm=y|n(default)
```
* 编译使用本地指令集
```bash 
--native=y|n(default)
```
* 选择工具链
```bash 
--sdk=ToolchainPath
```
* 静态链接
```bash
--static=y(default)|n
```
* 设置 sysroot
```bash
--sysroot=<path>
```
* 记录每一步消耗的时间
```bash 
--timer=y|n(default)
```
* 启用清洁器
```bash
--policies=build.sanitizer.address --policies=build.sanitizer.leak
```
