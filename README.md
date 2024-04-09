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
        href="http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=VwOd_SiQ31UIiX_QtI047ngYYgkzvvlB&authKey=HJecYKgB1HQCtIplBkNjeSxlat8OgNXtk47QURCS6y2c7dAifwHaKZaURIci6yE3&noverify=0&group_code=137136029">
        <img src="https://img.shields.io/badge/chat-on%20QQ-red.svg" , alt="QQ" />
    </a>
    <a
        href="https://discord.gg/xkvGy79e">
        <img src="https://img.shields.io/badge/chat-on%20Discord-7289da.svg" , alt="Discord" />
    </a>
</div>

[[中文]](README_CN.md)

## Contact Us

- [Discord](https://discord.gg/xkvGy79e)
- QQ: [137136029](http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=VwOd_SiQ31UIiX_QtI047ngYYgkzvvlB&authKey=HJecYKgB1HQCtIplBkNjeSxlat8OgNXtk47QURCS6y2c7dAifwHaKZaURIci6yE3&noverify=0&group_code=137136029)
- repositories: [Gitee](https://gitee.com/UltiELF/ulti-wvm), [GitHub](https://github.com/UltiELF/ulti-wvm)

## Introduce
Ultimate WebAssembly Virtual Machine

## Features
### Supports multiple platforms
* (x86\_64, i386, aarch64, arm)-windows-(gnu, msvc) (support UCRT and MSVCRT)
* (x86\_64, i386, aarch64, arm, loongarch64, riscv64, mips64, powerpc64, etc.)-linux-(gnu, musl, llvm, mlibc, uclibc, avrlibc, etc.)
* (x86\_64, i386, aarch64, arm, powerpc, powerpc64, powerpc64le, powerpcspe, riscv64)-freebsd
* unknown-dragonflybsd
* unknown-netbsd
* unknown-openbsd
* unknown-bsd
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
* Currently, only MSVC 14.30+ GCC 14+ and LLVM 18+ are supported.
* libcxx[\*](https://github.com/llvm/llvm-project/blob/main/libcxx%2Finclude%2Fversion) is not supported for now
* To compile systems compatible with Win10 (default) or below, please add parameters
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
* Enable Sanitizer
```bash 
--policies=build.sanitizer.address --policies=build.sanitizer.leak
```

