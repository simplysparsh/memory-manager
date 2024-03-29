# Memory-Manager
Implement memory management schemes to manage a chunk of memory for a single thread system.

## Description
In this project, we will implement four memory allocation schemes to manage a chunk of memory and mimic malloc() and free() functions for a single thread system:

- First Fit
- Best Fit
- Worst Fit and
- Buddy System

The `main()` in the testcases will first use a standard glibc `malloc()` call to allocate a large amount of memory from the system. The specific amount of memory that your system will handle will be defined by a parameter `mem_size` which is a parameter of the `setup() function`. The code will then use this memory to form and return the requested smaller chunks of memory to the subsequent `my_malloc()` calls.

##Requesting a large memory chunk in the beginning 
One of the most popular memory management strategies is to request for large memory chunks during program startup and then intermittently during code execution. Memory allocation requests for individual data structures are carved out from these chunks. This results in far fewer system calls and boosts the performance time.
