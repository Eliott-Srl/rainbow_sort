#pragma once

#include <stdlib.h>
#include <stdio.h>

//little function so it's easier to debug
#define ON_ERROR_EXIT(cond, message) \
do { \
    if((cond)) { \
        std::cout << "Error in function: " << __func__ << " at line " << __LINE__ << std::endl; \
        perror(message); \
        exit(1); \
    } \
} while (0)