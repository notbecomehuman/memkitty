<p align="center">
  <img src="./assets/banner.png" alt="memkitty" />
</p>

# Native Windows process memory toolkit for Node.js.


## Installation

```bash
npm install memkitty
```
During installation the native addon will be compiled using `node-gyp`.

Make sure Visual Studio C++ Build Tools are installed.

## Requirements

### Build Tools

This package contains a native Node.js addon and requires
Visual Studio C++ Build Tools to compile.

Install:

- Visual Studio 2022
- Desktop development with C++

or

- Build Tools for Visual Studio 2022
- Desktop development with C++

## Example

```ts
import { ProcessManager } from 'memkitty';

const pid =
    ProcessManager.getPidsByName(
        'notepad.exe'
    )[0];

const process =
    new ProcessManager(
        pid,
        'x86'
    );

process.open();

const base =
    process.getBaseAddress();

console.log(base);
```

## Features

- Process enumeration
- Open process
- Read pointer chains
- Read Int32
- Read UInt32
- Read Int64
- Read UInt64
- Read Float
- Read WString
- Read raw bytes
- Allocate remote memory
- Inject shellcode

## Supported Platforms

- Windows x86
- Windows x64
- Windows ARM64