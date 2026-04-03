# Kyrogram

Modern, fast, and secure Telegram client written entirely in C.

## Author
**pon4ikisdonut (KyroSystems NUA)**

## Features
- **MTProto 2.0:** Integrated through TDLib (JSON Interface).
- **Themes:** Supports Liquid Glass, Material You, Fluent, and more.
- **Security:** AES-256 encrypted account storage.
- **Cross-platform:** Windows, Linux, MacOS, KyroOS (Ready), and more.
- **Performance:** Built on Raylib for high-end rendering with low overhead.

## Versioning System
`MAJOR.MINOR.PATCH-STATUS` (Current: 1.0.0-Stable)

## Build Requirements
- CMake 3.10+
- Raylib
- mbedTLS
- TDLib

## How to build
```bash
mkdir build && cd build
cmake ..
make
```

