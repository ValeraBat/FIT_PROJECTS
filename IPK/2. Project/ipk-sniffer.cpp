// standart C stuff
#include <stdio.h>      // printf()
#include <string.h>     // strcpy(), strcat()
#include <stdlib.h>     // EXIT_SUCCES, EXIT_FAILURE macros
#include <unistd.h>     // getopt()
#include <ctype.h>      // isprint()
#include <stdbool.h>    // type boolean
#include <sys/time.h>   // time miliseconds
#include <time.h>       // time()

// used for exiting the program (with ctrl+c etc.) with proper clear
#include <signal.h>

// Definitions for certain ether types
#define IPv4_TYPE 2048
#define IPv6_TYPE 34525