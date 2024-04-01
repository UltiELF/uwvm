<div style="text-align:center">
    <img src="documents/images/logo_256x256.png" , alt="logo" />
    <h1>Ultimate WebAssembly Virtual Machine</h1>
    <a href="LICENSE.md">
        <img src="https://img.shields.io/badge/License-Apache%202.0-green.svg" , alt="License" />
    </a>
    <a href="https://en.cppreference.com">
        <img src="https://img.shields.io/badge/language-c++23-blue.svg" ,alt="cppreference" />
    </a>
    <a
        href="http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=XZB6BqBhkGX9RI8lNIvPRQpqjIHYDCpZ&authKey=OPmC%2FnGNXThLAV7IKmEQ57uiQCTfb8EraImxCWzVgq9%2FmdgxGU6rA3wZB%2BbCVxjq&noverify=0&group_code=284938376">
        <img src="https://img.shields.io/badge/chat-on%20QQ-red.svg" , alt="QQ" />
    </a>
    <a
        href="https://discord.gg/xkvGy79e">
        <img src="https://img.shields.io/badge/chat-on%20Discord-purple.svg" , alt="Discord" />
    </a>
</div>

[[中文]](README_CN.md)

## Contact Us

- QQ: [284938376](http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=XZB6BqBhkGX9RI8lNIvPRQpqjIHYDCpZ&authKey=OPmC%2FnGNXThLAV7IKmEQ57uiQCTfb8EraImxCWzVgq9%2FmdgxGU6rA3wZB%2BbCVxjq&noverify=0&group_code=284938376)
- repositories: [Gitee](https://gitee.com/UltiELF/ulti-wvm), [GitHub](https://github.com/UltiELF/ulti-wvm)

## Introduce
Ultimate WebAssembly Virtual Machine

## Features
### Supports multiple platforms
* (x86\_64, i386, aarch64, arm)-windows-(gnu, msvc) (support UCRT and MSVCRT)
* (x86\_64, i386, aarch64, arm, loongarch64, riscv64, mips64, powerpc64, etc.)-linux-(gnu, musl, llvm, mlibc, uclibc, avrlibc, etc.)
* unknown-FreeBSD 
* unknown-DragonFlyBSD 
* unknown-NetBSD 
* unknown-OpenBSD 
* unknown-sun
* unknwon-apple-darwin
* i386-msdosdjgpp
* wasm32-wasip1, wasm32-wasip2, wasm64-wasip1, wasm64-wasip2 (bootstrap)
* etc.

## How to build
1. Install [[xmake]](https://github.com/xmake-io/xmake/)
2. Install [[gcc14+]](https://github.com/trcrsired/gcc-releases/releases) or [[llvm18+]](https://github.com/trcrsired/llvm-releases/releases)
3. Build
```bash
$ xmake 
$ xmake install -o OutputPath 
```
4. Build parameters
```bash
$ xmake f -m [release|releasedbg|debug] -p [windows|mingw|macosx|linux|iphoneos ..] -a [x86_64|i386|arm|aarch64 ..] --cppstdlib=[default|libstdc++|libc++] ..
```
* Currently, only MSVC 14.3+ GCC 14+ and LLVM 18+ are supported.
* Currently, Windows only supports Windows NT 3.1+ systems and does not support Windows 9x(Possible future support). To compile systems compatible with Win10 (default) or below, please add parameters
```bash 
--min-win32-sys=[WIN10|WINBLUE|WIN8|WIN7|WS08|VISTA|WS03|WINXP] 
```
* For Windows 9x (Not supported yet)
```bash 
--min-win32-sys=[WINME|WIN98|WIN95]
```
* Using the llvm toolchain
```bash 
--use-llvm=y|n(default)
```
* Compile using local instruction sets
```bash 
--native=y|n(default)
```
* Select toolchain
```bash 
--sdk=ToolchainPath
```
* Static linking
```bash 
--static=y(defalut)|n
```
* Set sysroot
```bash 
--sysroot=<path>
```
