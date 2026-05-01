# Custom Shell
Building a custom linux shell for learning

## Features
* Execute external commands using `fork` and `execvp`
* Built-in commands:
    - `cd`
    - `exit`
    - `jobs`
    - `fg`
    - `bg`

* I/O Redirection:
    - Input: `<`
    - Output: `>`
* Pipe support: `|`
* Background processes: `&`
* Double quotes and escape handling: `""`, `\`
* Single quotes handling: `''`
* Process groups
* Arrow keys for navigation

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
* Handles double quotes with escapes, single quotes
* Syntax error handling
* Process groups and job control

## TODO
* update background job status after finish
