
#include "util.h"

#include <errno.h>


int last_file_update(const char *path, time_t * ret) {
    struct stat file_stat;
    int err = stat(path, &file_stat);
    if (err != 0) {
        return -1;
    }
    *ret = file_stat.st_mtime;

    return 0;
}
