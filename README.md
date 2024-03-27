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
* (x86\_64, i386, aarch64, arm)-windows-gnu (support UCRT and MSVCRT)
* (x86\_64, i386, aarch64, arm, loongarch64, riscv64, mips64, powerpc64, etc.)-linux-(gnu, musl, llvm, mlibc, uclibc, avrlibc, etc.)
* unknown-FreeBSD 
* unknown-DragonFlyBSD 
* unknown-NetBSD 
* unknown-OpenBSD 
* unknown-sun
* unknwon-apple-darwin
* i386-msdosdjgpp
* etc.

## How to build
1. Install [[xmake]](https://github.com/xmake-io/xmake/)
2. Install [[gcc14+]](https://github.com/trcrsired/gcc-releases/releases) or [[llvm18+]](https://github.com/trcrsired/llvm-releases/releases)
3. Build
```bash
On Windows:

> xmake f -p mingw
> xmake 
> xmake install -o OutputPath 
```
```bash
On Other Platforms:

$ xmake 
$ xmake install -o OutputPath 
```
4. Build parameters
```bash
$ xmake f -m [release|releasedbg|debug] -p [mingw|macosx|linux|iphoneos ..] -a [x86_64|i386|aarch|aarch64|loongarch64 ..] --cppstdlib=[default|libstdc++|libc++] ..
```
* Currently, only GCC 14+and LLVM 18+are supported, and MSVC is not currently supported. Therefore, for local compilation on Windows, please set the platform to mingw (unknown windows gnu) instead of the default Windows (unknown windows msmc)
* Currently, Windows only supports Windows NT 3.1+ systems and does not support Windows 9x(Possible future support). To compile systems compatible with Win10 (default) or below, please add parameters
```bash 
--min-win32-sys=[WIN10|WINBLUE|WIN8|WIN7|WS08|VISTA|WS03|WINXP] 
```
* For Windows 9x (Not supported yet)
```bash 
--min-win32-sys=[WINME|WIN98|WIN95]
```
* To use the llvm compiler, add parameters
```bash 
--use-llvm=y
```
* To perform cross compilation, please turn off compiling the local instruction set
```bash 
--native=n
```
* Select toolchain
```bash 
--sdk=ToolchainPath
```
