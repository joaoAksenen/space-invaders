#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "types.h"
#include "error.h"
#include "file.h"

// 20 Mib, can probably change this to a higher value without issue
// Check your target platform
#define IO_READ_CHUNK_SIZE 2097152
#define IO_READ_ERROR_GENERAL "Error reading file: %s. errno: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"


// Adapted from https://stackoverflow.com/a/44894946 (not the chosen answer) by Nominal Animal
File engineFileRead(const char *path) {
    File file = { .isValid = false };

    // Opens file for reading in binary
    FILE *pFile = fopen(path, "rb");
    if (!pFile || ferror(pFile)) {
        ERROR_RETURN(file, IO_READ_ERROR_GENERAL, path, errno);
    }

    char *pData = NULL;
    char *pTmp;
    size_t used = 0;
    size_t size = 0;
    size_t n;

    while (true) {
        if ((used + IO_READ_CHUNK_SIZE + 1) > size) {
            size = used + IO_READ_CHUNK_SIZE + 1;

            // overflow check
            if (size <= used) {
                free(pData);
                ERROR_RETURN(file, "Input file too large: %s\n", path);
            }

            // allocate another chunk's worth of memory for the data buffer
            pTmp = realloc(pData, size);
            if (!pTmp) {
                free(pData);
                ERROR_RETURN(file, IO_READ_ERROR_MEMORY, path);
            }

            pData = pTmp;
        }

        // reads the data and returns the amount of bytes read
        n = fread(pData + used, 1, IO_READ_CHUNK_SIZE, pFile);
        if (n == 0) // end of file
            break;

        used += n;
    }

    if (ferror(pFile)) {
        free(pData);
        ERROR_RETURN(file, IO_READ_ERROR_GENERAL, path, errno);
    }

    // add null termination to data
    pTmp = realloc(pData, used + 1);
    if (!pTmp) {
        free(pData);
        ERROR_RETURN(file, IO_READ_ERROR_MEMORY, path);
    }
    pData = pTmp;
    pData[used] = 0;

    file.data = pData;
    file.length = used;
    file.isValid = true;

    return file;
}


int engineFileWrite(void *pBuffer, size_t size, const char *path) {
    FILE *pFile = fopen(path, "wb");
    if (!pFile || ferror(pFile))
        ERROR_RETURN(1, "Cannot write file: %s.\n", path);

    size_t chunks_written = fwrite(pBuffer, size, 1, pFile);
    fclose(pFile);

    if (chunks_written != 1)
        ERROR_RETURN(1, "Write error." 
                        "Expected 1 chink, got %zu. \n", chunks_written);

    return 0;
}