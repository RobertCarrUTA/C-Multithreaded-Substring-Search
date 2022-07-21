*README.md is still under construction*
# Multithreaded Substring Search in C

## Table of Contents
- Multithreaded Substring Search in C
  * [Compile Instructions](#compile-instructions)
  * [Requirements for Using Your Own Textfile](#requirements-for-using-your-own-textfile)
  * [Evaluation of Metrics and Interpretation of Collected Data](#evaluation-of-metrics-and-interpretation-of-collected-data)
    * [Hamlet Runtime Graph](#hamlet-runtime-graph)
    * [Shakespeare Runtime Graph](#shakespeare-runtime-graph)

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

## Evaluation of Metrics and Interpretation of Collected Data
Below are two graphs that help visualize the runtimes in respect to how many threads were running on the application:

### Hamlet Runtime Graph
![alt text](https://github.com/RobertCarrUTA/C-Multithreaded-Substring-Search/blob/main/images/hamlet%20runtime%20graph.png)


Figure 1. Program Runtime (hamlet.txt). The above graph shows the runtimes for different thread counts when running substring.c with hamlet.txt. Runtime was calculated by taking three separate runs and averaging them out.

### Shakespeare Runtime Graph
![alt text](https://github.com/RobertCarrUTA/C-Multithreaded-Substring-Search/blob/main/images/shakespeare%20runtime%20graph.png)


Figure 2. Program Runtime (shakespeare.txt). The above graph shows the runtimes for different thread counts when running substring.c with shakespeare.txt. Runtime was calculated by taking three separate runs and averaging them out.
