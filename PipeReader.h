#ifndef PIPE_READER_H
#define PIPE_READER_H
#include<cstdlib>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<string>

extern "C"
{
    static char* readall(FILE* source, size_t* length)
    {
        char* data = NULL;
        size_t  size = 0;
        size_t  used = 0;
        size_t  n;

        /* If we have a place to store the length,
           we initialize it to zero. */
        if (length)
            *length = 0;

        /* Do not attempt to read the source, if it is
           already in end-of-file or error state. */
        if (feof(source) || ferror(source))
            return NULL;

        while (1) {

            /* Ensure there is unused chars in data. */
            if (used >= size) {
                const size_t  new_size = (used | 65535) + 65537 - 32;
                char* new_data;

                new_data = (char*)realloc(data, new_size);
                if (!new_data) {
                    /* Although reallocation failed, data is still there. */
                    free(data);
                    /* We just fail. */
                    return NULL;
                }

                data = new_data;
                size = new_size;
            }

            /* Read more of the source. */
            n = fread(data + used, 1, size - used, source);
            if (!n)
                break;

            used += n;
        }

        /* Read error or other wonkiness? */
        if (!feof(source) || ferror(source)) {
            free(data);
            return NULL;
        }

        /* Optimize the allocation. For ease of use, we append
           at least one nul byte ('\0') at end. */
        {
            const size_t  new_size = (used | 7) + 9;
            char* new_data;

            new_data = (char*)realloc(data, new_size);
            if (!new_data) {
                if (used >= size) {
                    /* There is no room for the nul. We fail. */
                    free(data);
                    return NULL;
                }
                /* There is enough room for at least one nul,
                   so no reason to fail. */
            }
            else {
                data = new_data;
                size = new_size;
            }
        }

        /* Ensure the buffer is padded with nuls. */
        memset(data + used, 0, size - used);

        /* Save length, if requested. */
        if (length)
            *length = used;

        return data;
    }

}
#endif