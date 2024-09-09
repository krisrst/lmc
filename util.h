
#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int last_file_update(const char *path, time_t * ret);

#endif // UTIL_H
