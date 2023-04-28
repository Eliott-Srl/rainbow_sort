#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define ON_ERROR_EXIT(cond, message) \
do { \
    if((cond)) { \
        std::cout << "Error in function: " << __func__ << " at line " << __LINE__ << std::endl; \
        perror(message); \
        exit(1); \
    } \
} while (0)

static inline bool str_ends_in(const char *str, const char *ends) {
    size_t str_len = strlen(str);
    size_t ends_len = strlen(ends);
    const char *pos = strstr(str, ends);
    return (pos != NULL) && (pos + ends_len == str + str_len);
}