
#include "log.h"


int log_init(struct lmc * lmc){

    lmc->log = fopen("log.txt", "w+");
    if( ! lmc->log ){
        fprintf(stderr, "Could not open log file!\n");
        return -1;
    }

    return 0;
}

void log_destroy(struct lmc * lmc){

    fclose(lmc->log);
}

int log_printf(struct lmc * lmc, const char *fmt, ...){

    int length;
    char buf[4096];
    va_list va;
    va_start(va, fmt);

    length = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    fwrite(buf, sizeof(char), length, lmc->log);
    fflush(lmc->log);

    return length;
}
