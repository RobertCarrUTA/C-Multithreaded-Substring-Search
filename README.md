# Multithreaded Substring Search in C

## Table of Contents
- Multithreaded Substring Search in C
  * [Compile Instructions](#compile-instructions)
  * [Requirements for Using Your Own Textfile](#requirements-for-using-your-own-textfile)
  * [Evaluation of Metrics and Interpretation of Collected Data](#evaluation-of-metrics-and-interpretation-of-collected-data)
    * [Hamlet Runtime Graph](#hamlet-runtime-graph)
    * [Shakespeare Runtime Graph](#shakespeare-runtime-graph)
    * [Interpretation of Collected Data](#interpretation-of-collected-data)
    * [Detailed Program Description](#detailed-program-description)

## About the Project
In this program, a substring search is performed in text files in a multithreaded manner. Multithreading was used to improve the efficiency of searching for substrings within large strings (there are over 4,318,6700 characters in [shakespeare.txt](https://github.com/RobertCarrUTA/C-Multithreaded-Substring-Search/blob/main/shakespeare.txt)!). [POSIX thread libraries](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html) were used to support multiple threads. POSIX provides standard C/C++ thread APIs. New concurrent processes were spawned with this method.

## Compile Instructions
These compile instructions are for a Ubuntu Operating System
* compile: gcc -pthread -o substring substring.c -lm
* compile with extra warnings: gcc -Wall -pthread -o substring substring.c -lm
* run: ./substring hamlet.txt
  * A filename in the current directory can be substituted for hamlet.txt when running. However, it must meet [the requirements for using your own text file](#requirements-for-using-your-own-textfile)

## Requirements for Using Your Own Textfile
Running this program requires a text file containing the entire string to be searched on the first line. To perform the search, the second line must contain the search substring. The following example can be found in the text file hamlet.txt:
* In line 1, you will find the following: all of hamlet in one line (the string must not contain any spaces)
* In line 2, you will find the following string: Hamlet
  * Hamlet is the substring that the program will search for in the string on line 1

## Evaluation of Metrics and Interpretation of Collected Data
Below are two graphs that help visualize the runtimes in respect to how many threads were running on the application (average of three runs was used to calculate the runtime):

### Hamlet Runtime Graph
![alt text](https://github.com/RobertCarrUTA/C-Multithreaded-Substring-Search/blob/main/images/hamlet%20runtime%20graph.png)


Figure 1. Program Runtime (hamlet.txt). The above graph shows the runtimes for different thread counts when running substring.c with hamlet.txt.

### Shakespeare Runtime Graph
![alt text](https://github.com/RobertCarrUTA/C-Multithreaded-Substring-Search/blob/main/images/shakespeare%20runtime%20graph.png)


Figure 2. Program Runtime (shakespeare.txt). The above graph shows the runtimes for different thread counts when running substring.c with shakespeare.txt.

### Interpretation of Collected Data
Performance improvements between single-threaded and multithreaded programs were shown in the two graphs (Figure 1 and Figure 2). One and two threads had the biggest visible gains. However, this is misleading. There was more work being done by the program when only one thread was used. Calling thread functions combined with mutexes would result in a higher runtime average when there is no work that is split between threads. However, when four threads were used, the performance increase was greatest. For shakespeare.txt, moving from no threads to two threads resulted in a 65% improvement in performance, and moving from two to four threads resulted in an 80% improvement in performance. Although in raw gains the biggest cutdown in runtime was from no threads to two threads.

Though the results are minimal, they clearly show the advantage multithreaded applications may have regarding performance. It must be remembered, however, that there is an optimal thread count for any given application. Whenever possible, it would be better to find the optimal number of threads rather than push as many threads as possible.

### Detailed Program Description
This C program searches for a substring in a given input file using multiple threads in parallel. The number of threads is defined by NUM_THREADS. The program reads two strings from a file, s1 and s2, and then searches for occurrences of s2 in s1. The program reports the total number of occurrences of s2 in s1 and the execution time in milliseconds.

The program consists of three functions: main, readf, and num_substring.

The main function initializes the mutex and then creates multiple threads to search for substrings. The pthread_create function is used to create each thread and pass the thread function num_substring as an argument. The function also checks if a thread cannot be created and exits the program with a failure status if an error occurs. The pthread_join function is used to ensure each thread has finished before printing the total number of substrings and the elapsed time.

The readf function first attempts to open the file specified by the filename argument using the fopen function. If the file cannot be opened, an error message is printed, and the function returns 0. The function then allocates memory for s1 and s2, reads the two strings from the file using fgets, and finds the length of each string using strlen. Finally, the function checks if either of the strings are NULL or if the length of s1 is less than s2, and returns an error if either condition is true.

The num_substring function is responsible for finding substrings and updating the global variable total using the mutex to ensure that multiple threads don't update the variable at the same time. The function also uses the global variable thread_counter to ensure that each thread has a unique ID. The function calculates the start and end positions of the portion of s1 that it will search for s2 based on the thread ID and the number of threads being used. It then searches for s2 in the specified portion of s1 and updates the total variable using the mutex if an occurrence is found.
