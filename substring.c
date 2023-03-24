#include <stdlib.h>   // Provides standard library functions for memory allocation, process control, and other basic functionalities
#include <stdio.h>    // Provides input and output functions for reading and writing to files and the console
#include <string.h>   // Provides functions for manipulating strings, such as copying and comparing strings
#include <sys/time.h> // Provides functions for working with time values, such as gettimeofday used in this program to measure the execution time of the program
#include <pthread.h>  // Provides functions and data structures for creating and managing threads, used in this program to implement parallelism
#include <unistd.h>   // Provides access to POSIX operating system API, including standard symbolic constants and types
#include <math.h>     // Provides mathematical functions, such as ceil used in this program to calculate the number of threads needed for parallel execution

#define MAX 5000000   // Maximum size for s1 and s2
#define NUM_THREADS 4 // Number of threads to use

int total          = 0; // Total number of substrings found
int thread_counter = 0; // Counter used to assign each thread a unique ID
int n1, n2;             // Lengths of s1 and s2
char *s1, *s2;          // Pointers to s1 and s2, s1 = entire file string, s2 = substring to look for in s1
FILE *fp;               // Pointer to the input file
pthread_mutex_t mutex;  // Mutex used to synchronize access to the "total" variable

/***********************************************************************************************************************
  @brief: readf attempts to read two strings (s1 and s2) from a file specified by the filename argument.
            It first attempts to open the file and returns an error message if it cannot be opened. It then allocates
            memory for s1 and s2, reads the two strings from the file using fgets(), and finds the length of each
            string using strlen(). Finally, the function checks if either of the strings are null or if the length of
            s1 is less than s2, and returns an error if either condition is true.
***********************************************************************************************************************/
int readf(char *filename)
{
   	// Attempt to open the file, if it doesn't exist, print an error message and return 0
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("ERROR: can’t open %s!\n", filename);
        return 0;
    }

   	// Allocate memory for string s1
    s1 = (char*) malloc(sizeof(char) *MAX);

   	// If memory allocation fails, print an error message and return -1
    if (s1 == NULL)
    {
        printf("ERROR: Out of memory!\n");
        return -1;
    }

   	// Allocate memory for string s2
    s2 = (char*) malloc(sizeof(char) *MAX);

   	// If memory allocation fails, print an error message and return -1
    if (s1 == NULL)
    {
        printf("ERROR: Out of memory\n");
        return -1;
    }

   	// Read s1 and s2 from the file
    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);
    n1 = strlen(s1);	    // Find the length of string s1
    n2 = strlen(s2) - 1;	// Find the length of string s2, excluding the newline character

   	// If any of the strings are NULL or the length of s1 is less than s2, return -1
    if (s1 == NULL || s2 == NULL || n1 < n2)
    {
        return -1;
    }
}

/***********************************************************************************************************************
  @brief: In summary, num_substring(void *ptr) is a function that searches for substrings of s2 in s1. It is designed to
            be run concurrently by multiple threads, with each thread responsible for a portion of s1. The function
            counts the number of times that s2 appears as a substring of s1, incrementing a global variable total and
            using a mutex to ensure that multiple threads don't update the variable at the same time. The function
            returns NULL.
***********************************************************************************************************************/
void *num_substring(void *ptr)
{
    // Get the ID of the current thread and increment the global thread counter
    int thread_id = thread_counter++;

    int i, j, k; // Loop variables
    int count;   // Keeps track of how many characters match between s1 and s2

    // Loop through a section of s1 based on the thread ID
    for (i = thread_id * ((n1 - n2) / NUM_THREADS); i < ((thread_id + 1) * ((n1 - n2) / NUM_THREADS)); i++)
    {
        count = 0;  // Reset count for each iteration of the outer loop

        // Check for a substring starting at position i
        for (j = i, k = 0; k < n2; j++, k++)
        {
            // If characters at s1 and s2 don't match, break out of the inner loop
            if (*(s1 + j) != *(s2 + k))
            {
                break;
            }
            else
            {
                count++; // Increment count if characters match
            }

            // If we've matched n2 characters, increment total count and release mutex
            if (count == n2)
            {
                pthread_mutex_lock(&mutex);
                total++;
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    return NULL;
}

/***********************************************************************************************************************
  @brief: The main function initializes a mutex, creates multiple threads to search for substrings, and then joins the
            threads to ensure they have finished before printing the total number of substrings and the elapsed time.
            The num_substring function is responsible for finding substrings and updating the global variable total
            using the mutex to ensure that multiple threads don't update the variable at the same time. The readf
            function reads two strings from a file and returns an error if either of the strings is NULL or the length
            of the first string is less than the second string.
***********************************************************************************************************************/
int main(int argc, char *argv[])
{
    pthread_t thread[NUM_THREADS];

    if (argc < 2)
    {
        printf("Error: You must pass in the datafile as a commandline parameter\n");
    }

    // Call the readf function with the first argument as the filename
    readf(argv[1]);

    struct timeval start, end;
    float mtime;
    int secs, usecs;

    // Get the start time of the program
    gettimeofday(&start, NULL);

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Print an error message if a thread can't be created and exit the program with a failure status
        if (pthread_create(&thread[i], NULL, num_substring, NULL))
        {
            perror("Error creating thread: ");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Print an error message if there's a problem with pthread_join
        if (pthread_join(thread[i], NULL))
        {
            perror("Problem with pthread_join on thread: ");
        }
    }

    // Print the total number of substrings found
    printf("The number of substrings is : %d\n", total);

    // Get the end time of the program
    gettimeofday(&end, NULL);

    secs  = end.tv_sec - start.tv_sec;             // Calculate the number of seconds elapsed
    usecs = end.tv_usec - start.tv_usec;           // Calculate the number of microseconds elapsed
    mtime = ((secs) *1000 + usecs / 1000.0) + 0.5; // Calculate the total elapsed time in milliseconds

    printf("Elapsed time is : %f milliseconds\n", mtime); // Print the elapsed time in milliseconds

    if (s1)
    {
        // Free the memory allocated for s1
        free(s1);
    }

    if (s2)
    {
        // Free the memory allocated for s2
        free(s2);
    }

    // Exit the pthread library
    pthread_exit(NULL);
 
    return 0;
}
