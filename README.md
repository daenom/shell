# Custom Shell
Building a custom linux shell for learning

## Features
* Execute external commands using `fork` and `execvp`
* Built-in commands:
    - `cd`
    - `exit`
* I/O Redirection:
    - Input: `<`
    - Output: `>`
* Single pipe support: `|`

## Getting Started

### Build

```bash
make
```

### Run

```bash
./shell
```

## Current Status

* Basic shell functionality implemented
* Modularized architecture
* Supports single pipe and redirection
