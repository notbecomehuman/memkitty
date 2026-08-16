<p align="center">
  <img src="./assets/banner.png" alt="memkitty" />
</p>

## Native Windows process memory toolkit for Node.js.

## Features

- Process discovery by executable name
- Process opening and management
- Automatic x86/x64 architecture detection
- Read Int32 / UInt32
- Read Int64 / UInt64
- Read Float
- Read UTF-16 strings
- Read raw bytes
- Read pointers
- Remote memory allocation
- Memory injection
- Inject and execute shellcode

---

## Installation

```bash
npm install memkitty
```
During installation the native addon will be compiled using `node-gyp`.

Make sure Visual Studio C++ Build Tools are installed.

---

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

---

## Quick Start

```ts
import { ProcessManager } from 'memkitty';

const pid =
    ProcessManager.getPidsByName(
        'example.exe'
    )[0];

if (!pid) {
    throw new Error(
        'Process not found'
    );
}

const process =
    new ProcessManager(
        pid,
        'auto'
    );

process.open();

const base =
    process.getBaseAddress();

const value =
    process.readInt32(
        base + 0x100n
    );

console.log(value);
```

---

## Types

### Address

```ts
type Address = bigint;
```

All memory addresses are represented as `bigint`.

Example:

```ts
const address =
    0x12345678n;
```

### ProcessArchitecture

```ts
type ProcessArchitecture =
    | 'auto'
    | 'x86'
    | 'x64';
```

| Value | Description |
| --- | --- |
| `auto` | Detect architecture automatically |
| `x86` | Force 32-bit pointers |
| `x64` | Force 64-bit pointers |

---

# ProcessManager API

## Constructor

```ts
new ProcessManager(
    pid: number,
    architecture?: ProcessArchitecture
)
```

Example:

```ts
const process =
    new ProcessManager(
        1234,
        'auto'
    );
```

---

## Static Methods

### getPidsByName

Finds running processes by executable name.

```ts
ProcessManager.getPidsByName(
    processName: string
): number[]
```

Example:

```ts
const pids =
    ProcessManager.getPidsByName(
        'elementclient.exe'
    );
```

---

## Process Control

### open

```ts
open(): boolean
```

Opens the target process.

### close

```ts
close(): void
```

Closes the process handle.

### isOpen

```ts
isOpen(): boolean
```

Returns whether the process is currently open.

### getArchitecture

```ts
getArchitecture(): ProcessArchitecture
```

Returns the resolved process architecture.

### getBaseAddress

```ts
getBaseAddress(): Address
```

Returns the process base address.

---

## Memory Reading

### readInt32

```ts
readInt32(
    address: Address
): number
```

### readUInt32

```ts
readUInt32(
    address: Address
): number
```

### readInt64

```ts
readInt64(
    address: Address
): bigint
```

### readUInt64

```ts
readUInt64(
    address: Address
): bigint
```

### readFloat

```ts
readFloat(
    address: Address
): number
```

### readPointer

```ts
readPointer(
    address: Address
): Address
```

Reads a pointer using the target process architecture.

Example:

```ts
const ptr =
    process.readPointer(
        base + 0x100n
    );

const hp =
    process.readInt32(
        ptr + 0x20n
    );
```

### readBytes

```ts
readBytes(
    address: Address,
    size: number
): Buffer
```

### readWString

```ts
readWString(
    address: Address,
    length: number
): string
```

Reads a UTF-16 string.

---

## Remote Memory

### allocateMemory

```ts
allocateMemory(
    size: number
): Address
```

Allocates memory in the target process.

### inject

```ts
inject(
    address: Address,
    data: Buffer
): boolean
```

Writes bytes to the target process.

### injectAndExecute

```ts
injectAndExecute(
    address: Address,
    data: Buffer
): boolean
```

Writes bytes and executes the supplied code in the target process.

> This is a low-level API. Only use it with processes and code you are authorized to modify.

---

## API Summary

| Method | Returns |
|----------|----------|
| `getPidsByName()` | `number[]` |
| `open()` | `boolean` |
| `close()` | `void` |
| `isOpen()` | `boolean` |
| `getArchitecture()` | `ProcessArchitecture` |
| `getBaseAddress()` | `Address` |
| `readInt32()` | `number` |
| `readUInt32()` | `number` |
| `readInt64()` | `bigint` |
| `readUInt64()` | `bigint` |
| `readFloat()` | `number` |
| `readPointer()` | `Address` |
| `readBytes()` | `Buffer` |
| `readWString()` | `string` |
| `allocateMemory()` | `Address` |
| `inject()` | `boolean` |
| `injectAndExecute()` | `boolean` |

---

## License

MIT