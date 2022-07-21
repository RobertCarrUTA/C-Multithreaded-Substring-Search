*README.md is still under construction*
# Multithreaded Substring Search in C

## Table of Contents
- Multithreaded Substring Search in C
  * [Compile Instructions](#compile-instructions)
  * [Requirements for Using Your Own Textfile](#requirements-for-using-your-own-textfile)

## About the Project
In this program, substring searches are performed in text files in a multithreaded manner. Multithreading was used to improve the efficiency of searching for substrings within large strings (there are over 4,318,6700 characters in [shakespeare.txt](https://github.com/RobertCarrUTA/C-Multithreaded-Substring-Search/blob/main/shakespeare.txt)!). [POSIX thread libraries](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html) were used to support multiple threads. POSIX provides standard C/C++ thread APIs. New concurrent processes were spawned with this method.

## Compile Instructions
These compile instructions are for a Ubuntu Operating System
* compile: gcc -pthread -o substring substring.c
* compile with extra warnings: gcc -Wall -pthread -o substring substring.c
* run: ./substring hamlet.txt
  * A filename in the current directory can be substituted for hamlet.txt when running. However, it must meet [the requirements for using your own text file](#requirements-for-using-your-own-textfile)

## Requirements for Using Your Own Textfile
Running this program requires a text file containing the entire string to be searched on the first line. To perform the search, the second line must contain the search substring. The following example can be found in the text file hamlet.txt:
* In line 1, you will find the following: all of hamlet in one line (the string must not contain any spaces)
* In line 2, you will find the following string: Hamlet
  * Hamlet is the substring that the program will search for in the string on line 1
