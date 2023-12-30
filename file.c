/**
 * @author swjang702@gmail.com
 * @brief simple file stream handler
 * 
 * $ gcc -g -Wall -O2 -o file file.c
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef FILE * (*func_open)(const char *);
typedef int    (*func_close)(FILE *);

typedef struct {
    const char *path;
    FILE       *stream;
    func_open   open;
    func_close  close;
} file;

FILE * file_open(const char *path)
{
    FILE *stream;
    int err;
    
    stream = fopen(path, "r");
    if (!stream) {
        err = errno;
        fprintf(stderr, "!! %s:%d: %d (%s); path=%s\n", __FUNCTION__, __LINE__, err, strerror(err), path);
    }

    return stream;
}

int file_close(FILE *stream)
{
    int ret = 0;
    int err;

    if (!stream) {
        fprintf(stderr, "!! %s:%d: NULL stream\n", __FUNCTION__, __LINE__);
    	return -EFAULT;
    }

    ret = fclose(stream);
    if (0 != ret) {
        err = errno;
        fprintf(stderr, "!! %s:%d: %d (%s)\n", __FUNCTION__, __LINE__, err, strerror(err));
    }

    return ret;
}

int main()
{
    file f = {"test.txt", NULL, file_open, file_close};
    char str[128];

    f.stream = f.open(f.path);

    if (f.stream) {
        fscanf(f.stream, "%s", str);
        puts(str);
        f.close(f.stream);
    }

    return 0;
}
