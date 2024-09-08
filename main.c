
#include "gui.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

static int keep_running = 1;

void sighandler(int sig){
    keep_running = 0;
}

int main(int argc, char *argv[]){

    int ret, i;
    struct lmc * lmc;

    lmc = malloc(sizeof(struct lmc));
    assert(lmc);
    memset(lmc, 0, sizeof(struct lmc));

    for(i=0; i<100; i++) lmc->memory[i] = 0;

    lmc->focus = FOCUS_NONE;
    lmc->state = LMC_CODING;

    lmc->output_mem = calloc(ASSEMBLER_MAX_CHAR, 1);
    assert(lmc->output_mem);

    lmc->input_mem = calloc(ASSEMBLER_MAX_CHAR, 1);
    assert(lmc->input_mem);

    sprintf(lmc->assembler_mem[0], "INP\0");
    sprintf(lmc->assembler_mem[1], "OUT\0");
    sprintf(lmc->assembler_mem[2], "HLT\0");
    sprintf(lmc->assembler_mem[3], "test ADD 9\0");
    sprintf(lmc->assembler_mem[4], "ADD test\0");

    lmc->majver = 0;
    lmc->minver = 1;

    ret = log_init(lmc);
    if(ret) goto exit0;

    ret = gui_init(lmc);
    if(ret) goto exit1;

    log_printf(lmc, "test!\n");

    while(keep_running) usleep(10E3);

    lmc->shutdown = 1;

    gui_destroy(lmc);
exit1:
    log_destroy(lmc);

exit0:
    free(lmc->assembler_mem);
    free(lmc);

    return 0;
}
