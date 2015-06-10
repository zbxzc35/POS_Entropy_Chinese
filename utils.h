/* ========================================================================== */
/* File: utils.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains helpful utility macros.
 *
 */
/* ========================================================================== */
#ifndef UTILS_H
#define UTILS_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdio.h>                           // fprintf
#include <stdlib.h>                          // exit

// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros

// Print s with the source file name and the current line number to fp.
#define LOG(fp,s)  fprintf((fp), "Log: [%s:%d] %s.\n", __FILE__, __LINE__, (s))

// Print error to fp and exit if s evaluates to false.
#define ASSERT_FAIL(fp,s) do {                                          \
        if(!(s))   {                                                    \
            fprintf((fp), "Error: [%s:%d] assert failed.\n", __FILE__, __LINE__); \
            errorFlag = 1;                                              \
            goto cleanup;                                               \
        }                                                               \
    } while(0)

// malloc a new data structure t
#define NEW(t) malloc(sizeof(t))

// Print error to fp and exit if s is NULL.
#define MALLOC_CHECK(fp,s)  do {                                        \
        if((s) == NULL)   {                                             \
            fprintf((fp), "Error: [%s:%d] out of memory.\n", __FILE__, __LINE__);  \
            errorFlag = 1;                                              \
            goto cleanup;                                               \
        }                                                               \
    } while(0)

// Set memory space starts at pointer \a n of size \a m to zero.
#define BZERO(n,m)  memset(n, 0, m)

#endif // UTILS_H
