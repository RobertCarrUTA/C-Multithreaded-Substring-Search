*README.md is still under construction*
# Multithreaded Substring Search in C

## Table of Contents
- Multithreaded Substring Search in C
  * [Compile Instructions](#compile-instructions)
  * [Requirements for Using Your Own Textfile](#requirements-for-using-your-own-textfile)

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
