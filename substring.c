#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define MAX 5000000   // Maximum size for s1 and s2
#define NUM_THREADS 4

int readf(char*);
void *num_substring(void*);

int total          = 0; // Total number of substrings found
int thread_counter = 0; // Counter used to assign each thread a unique ID
int n1, n2;             // Lengths of s1 and s2
char *s1, *s2;          // Pointers to s1 and s2, s1 = larger string for s2 to search in, s2 = substring to look for in s1
FILE *fp;               // Pointer to the input file

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex used to synchronize access to the "total" variable

/***********************************************************************************************************************
  The main function initializes a mutex, creates multiple threads to search for substrings, and then joins the
    threads to ensure they have finished before printing the total number of substrings and the elapsed time.
    The num_substring function is responsible for finding substrings and updating the global variable total
    using the mutex to ensure that multiple threads don't update the variable at the same time. The readf
    function reads two strings from a file and returns an error if either of the strings is NULL or the length
    of the first string is less than the second string
***********************************************************************************************************************/
int main(int argc, char *argv[])
{
    pthread_t thread[NUM_THREADS];

    if (argc < 2)
    {
        printf("Error: You must pass in the datafile as a commandline parameter\n");
    }

    readf(argv[1]);

    struct timeval start, end;
    float mtime;
    int secs, usecs;

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

    printf("The number of substrings is : %d\n", total);

    gettimeofday(&end, NULL);

    secs  = end.tv_sec - start.tv_sec;             // Calculate the number of seconds elapsed
    usecs = end.tv_usec - start.tv_usec;           // Calculate the number of microseconds elapsed
    mtime = ((secs) *1000 + usecs / 1000.0) + 0.5; // Calculate the total elapsed time in milliseconds

    printf("Elapsed time is : %f milliseconds\n", mtime); // Print the elapsed time in milliseconds

    if (s1)
    {
        free(s1);
    }

    if (s2)
    {
        free(s2);
    }

    // Exit the pthread library
    pthread_exit(NULL);

    return 0;
}

/***********************************************************************************************************************
  readf attempts to read two strings (s1 and s2) from a file specified by the filename argument.
    It first attempts to open the file and returns an error message if it cannot be opened. It then allocates
    memory for s1 and s2, reads the two strings from the file using fgets(), and finds the length of each
    string using strlen(). Finally, the function checks if either of the strings are null or if the length of
    s1 is less than s2, and returns an error if either condition is true
***********************************************************************************************************************/
int readf(char *filename)
{
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("ERROR: can't open %s!\n", filename);
        return 0;
    }

    s1 = (char*) malloc(sizeof(char) * MAX);

    // If memory allocation fails, print an error message, close the file, and return -1
    if (s1 == NULL)
    {
        printf("ERROR: Out of memory!\n");
        fclose(fp);
        return -1;
    }

    s2 = (char*) malloc(sizeof(char) * MAX);

    // If memory allocation fails, print an error message, free s1, close the file, and return -1
    if (s2 == NULL)
    {
        printf("ERROR: Out of memory\n");
        free(s1);
        fclose(fp);
        return -1;
    }

    // Read s1 and s2 from the file
    if (fgets(s1, MAX, fp) == NULL || fgets(s2, MAX, fp) == NULL)
    {
        printf("ERROR: Error reading from file!\n");
        free(s1);
        free(s2);
        fclose(fp);
        return -1;
    }

    n1 = strlen(s1);     // Find the length of string s1
    n2 = strlen(s2) - 1; // Find the length of string s2, excluding the newline character

    // If the length of s1 is less than s2, print an error message, free s1 and s2, close the file, and return -1
    if (n1 < n2)
    {
        printf("ERROR: Length of s1 is less than s2!\n");
        free(s1);
        free(s2);
        fclose(fp);
        return -1;
    }

    return 1;
}

/***********************************************************************************************************************
  num_substring(void *ptr) is a function that searches for substrings of s2 in s1. It is designed to
    be run concurrently by multiple threads, with each thread responsible for a portion of s1. The function
    counts the number of times that s2 appears as a substring of s1, incrementing a global variable total and
    using a mutex to ensure that multiple threads don't update the variable at the same time. The function
    returns NULL
***********************************************************************************************************************/
void *num_substring(void *ptr)
{
    // Get the ID of the current thread and increment the global thread counter
    int thread_id = __sync_fetch_and_add(&thread_counter, 1);

    int i, j, k; // Loop variables
    int count;   // Keeps track of how many characters match between s1 and s2

    /*
      Calculate the range of characters in s1 to search
        It is possible that some characters in s1 will not be searched if n1 - n2 is not
        divisible by NUM_THREADS. This can be fixed by rounding up the result of the
        division with the ceil function
    */
    int start = ceil((double)(n1 - n2 + 1) / NUM_THREADS * thread_id);
    int end   = ceil((double)(n1 - n2 + 1) / NUM_THREADS * (thread_id + 1));

    // Loop through a section of s1 based on the thread ID
    for (i = start; i < end; i++)
    {
        count = 0;

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
        }

        // If we've matched n2 characters, increment total count
        if (count == n2)
        {
            pthread_mutex_lock(&mutex);
            total++;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}
