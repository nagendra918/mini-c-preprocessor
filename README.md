# Mini C Preprocessor

A lightweight C-based preprocessor that performs basic source-code preprocessing.

## Features

- Remove single-line (`//`) comments
- Remove multi-line (`/* */`) comments
- Preserve comments inside string and character literals
- Process object-like `#define` macros
- Expand macro occurrences with identifier-boundary checking
- Process `#include` directives
- Support user-defined header files
- Support system headers from `/usr/include/`
- Support conditional compilation:
  - `#ifdef`
  - `#ifndef`
  - `#else`
  - `#endif`
- Handle include guards
- Dynamic memory management using `malloc()` and `realloc()`
- Modular source-code organization

## Project Structure

```text
mini-c-preprocessor/
│
├── src/
│   ├── main.c
│   ├── preprocessor.c
│   ├── macro.c
│   ├── include.c
│   ├── conditional.c
│   └── comment.c
│
├── include/
│   ├── preprocessor.h
│   ├── macro.h
│   ├── include.h
│   ├── conditional.h
│   └── comment.h
│
├── examples/
├── tests/
├── Makefile
├── README.md
└── .gitignore
```

## How It Works

The input C source file is read into a dynamically allocated buffer and processed through multiple stages.

```text
Input C file
     ↓
Read into buffer
     ↓
Remove comments
     ↓
Process #include
     ↓
Process conditional compilation
     ↓
Process #define / macros
     ↓
Preprocessed output
```

## Build

### Using GCC

```bash
gcc src/main.c src/preprocessor.c src/comment.c src/macro.c src/include.c src/conditional.c -I include -o mini_cpp
```

### Using Make

```bash
make
```

## Usage

```bash
./mini_cpp <source_file>
```

Example:

```bash
./mini_cpp examples/basic.c
```

On Windows:

```powershell
.\mini_cpp.exe examples/basic.c
```

## Example

### Input

```c
#define MAX 100

// This comment is removed

#ifdef MAX
int value = MAX;
#else
int value = 0;
#endif
```

### Output

```c
int value = 100;
```

## Testing

The project contains separate example and test files.

### Examples

The `examples/` directory contains sample input files demonstrating the supported preprocessing features.

### Tests

The `tests/` directory contains test cases for:

- Comment removal
- Macro expansion
- Conditional compilation
- Header inclusion
- Include guards
- Full feature integration

## Limitations

This is a lightweight educational implementation and is not a complete replacement for the standard C preprocessor.

- Supports object-like macros only
- Function-like macros are not supported
- Macro arguments are not supported
- `#if` and `#elif` expression evaluation are not supported
- Full macro recursion is not supported
- System header lookup currently uses `/usr/include/`
- Not all standard C preprocessing rules are implemented

## Technologies

- C
- GCC
- Make
- File Handling
- Dynamic Memory Allocation
- Pointers
- String Processing

## Purpose

This project was developed to understand the basic working of a C preprocessor while practicing modular C programming, dynamic memory management, pointers, file handling, and source-code parsing.
