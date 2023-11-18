#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct File {
    char *data;
    size_t length;
    bool isValid;
} File;

File engineFileRead(const char *path);
int engineFileWrite(void *pBuffer, size_t size, const char *path);
