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
* Pipe support: `|`
* Background processes : `&`

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
* Supports pipe and redirection
* Supports background process creation
