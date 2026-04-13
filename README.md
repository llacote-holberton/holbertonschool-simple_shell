# Simple Shell: Command Line Interface to interact with your OS.

## Table of content
FIXME

## Overview
FIXME

### Summary
Simple Shell is a simple command line interface program allowing user to send simple commands to manipulate files and get information on OS.
It supports the most essential features a user could expect: support of user $PATH to find programs, propagation of command arguments,
 error handling, interactive and non-interactive mode.

### Copyright
This program has been developed by...
* Soufiane 	Filali
* Laurent Lacôte

FIXME (license)

## How to install and run
### Prerequisites
You must have the packages/tools related to C
  installed on your system, as well as a "compiler"
  (program used to "prepare" this tool for your machine).
In case that helps here are online resources to install
  those tools depending on your operating system
  * Windows: https://learn.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line?view=msvc-170
     and https://nullprogram.com/blog/2016/06/13/
  * Mac: https://www.cs.auckland.ac.nz/~paul/C/Mac/
  * Linux (deb package system): https://jvns.ca/blog/2025/06/10/how-to-compile-a-c-program/

### 1. Downloading
If you have git and are comfortable with command line,
  you can simply open one and go to the directory in which you want calculator to be.
  Then run (without the quotes) `"git clone https://github.com/llacote-holberton/holbertonschool-simple_shell.git"`
Otherwise you can simply download a zip containing all projects file
  by following [this url](https://github.com/llacote-holberton/holbertonschool-simple_shell/archive/refs/heads/main.zip)
  then unfolding it where you want on your computer.

### 2. Compiling
Open the terminal, go inside the project directory (the one created from git clone or unzip).
Then type the following command (without quotes if you're reading this as raw text).
`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`  


### 3. Starting program
Open a terminal place yourself in the directory in which you have downloaded/extracted the project then compiled an executable,  
and just type ./<executable-name>  (for example for the "demo executable" it would be `./custom_shell_demo.out`).

## How to use

### Usage overview
FIXME
For more detailed examples, confer the dedicated section [Examples of use](#examples-of-use).

### Features
FIXME

### Accessible help
FIXME

### Important rules and limitations
FIXME

### Examples of use

#### Valid examples
FIXME
| Use case                     | Source code                               | Output                 | N° of printed chars |
|------------------------------|-------------------------------------------|------------------------|---------------------|
| %c — Single character        | _printf("Character: %c\n", 'A');          | Character: A           | 13                  |

#### Failing examples
FIXME
Examples of faulty call ending in undefined behaviour.
```_printf("Hello %s, current credit: %d dollars.\n", 30, "Bob");```
  => Argument list provides variables of the right type, but the order mismatches the one of conversion commands.


## Technical information

### General architecture

FIXME

#### Process flow
![simple_shell_flowchart](./flowchart_hsh.png)

#### Notes on architecture choices
FIXME

#### Source code file structure
| Filename                     | Role                                                                                                 | Functions in file               |
|------------------------------|------------------------------------------------------------------------------------------------------|---------------------------------|
| main.h              | Custom header holding all "public-facing" functions, structures and constants.       | All prototypes except get_subprinters & delegate_to      |


FIXME

### Testing
FIXME


#### Technologies
<p align="left">
    <img src="https://img.shields.io/badge/C-a8b9cc?logo=&logoColor=black&style=for-the-badge" alt="C badge">
    <img src="https://img.shields.io/badge/GIT-f05032?logo=git&logoColor=white&style=for-the-badge" alt="Git badge">
    <img src="https://img.shields.io/badge/GITHUB-181717?logo=github&logoColor=white&style=for-the-badge" alt="GitHub badge">
    <img src="https://img.shields.io/badge/VALGRIND-purple?logo=v&logoColor=white&style=for-the-badge" alt="Valgrind badge">
    <img src="https://img.shields.io/badge/VIM-019733?logo=vim&logoColor=white&style=for-the-badge" alt="VIM badge">
    <img src="https://img.shields.io/badge/KDE-blue?logo=kde&logoColor=white&style=for-the-badge" alt="KDE badge">
</p>
