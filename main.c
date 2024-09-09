
#include "gui.h"
#include "log.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>

static int keep_running = 1;

static struct lmc lmc_;

void sighandler(int sig){
    keep_running = 0;
}

static void read_assembler_code(struct lmc * lmc){
    int i;
    char *line;
    size_t len;
    ssize_t n;

    line = NULL;
    len = 0;

    i = 0;

    fseek(lmc->code_fp, 0, SEEK_SET);

    while ((n = getline(&line, &len, lmc->code_fp)) != -1) {

        // Replace delimiting '\n' with string terminate
        line[n-1] = '\0';

        if( n < LINE_MAX_LEN){
            sprintf(lmc->assembler_mem[i++], "%s", line);
        }
        else{
            break;
        }

        if(i == 100) break;
    }

    if( line ) free(line);

    for(; i < 99; i++){
        sprintf(lmc->assembler_mem[i], "%s", "");
    }
}

static void print_help(){

    fprintf(stdout, "Usage: lmc assembler.txt <options>\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "assembler.txt\n");
    fprintf(stdout, " - A file containing your assembler code.\n");
    fprintf(stdout, " - Can be edited while this program is running.\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout, " -c    Clock frequency in hertz.\n");
    fprintf(stdout, "\n");
}

int main(int argc, char *argv[]){

    char c;
    int ret, i;
    struct lmc * lmc;

    lmc = &lmc_;
    memset(lmc, 0, sizeof(struct lmc));

    lmc->frq = 10; // 10 Hz should be plenty!

    if( argc < 2 ){
        print_help();
        return -1;
    }

    lmc->code_fp = fopen( argv[1], "r" );
    if( lmc->code_fp == NULL ){
        fprintf(stderr, "Could not open %s: %s\n", argv[1], strerror(errno));
        return -1;
    }

    if( last_file_update( argv[1], &lmc->code_mod ) ){
        fprintf(stderr, "Could not get file modification time for %s\n", argv[1]);
        fclose(lmc->code_fp);
        return -1;
    }

    while ((c = getopt (argc, argv, "hc:")) != -1){
        switch (c)
        {
            case 'h':
                print_help();
                return 0;
                break;
            case 'c':
                lmc->frq = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Unknown argument (%c)!\n", c);
                return -1;
                break;
        }
    }

    if( lmc->frq > 1E9 ){
        fprintf(stderr, "Clock frequency is capped at 1GHz.\n");
        return -1;
    }

    for(i=0; i<100; i++) lmc->memory[i] = 0;

    lmc->state = LMC_CODING;

    lmc->majver = 0;
    lmc->minver = 1;

    ret = log_init(lmc);
    if(ret) goto exit0;

    read_assembler_code(lmc);

    ret = gui_init(lmc);
    if(ret) goto exit1;

    // This loop waits for exit signal.
    // Regularly checks code file for updates.
    while(keep_running){
        time_t mod;
        if( last_file_update( argv[1], &mod )){
            log_printf(lmc, "last_file_update failed!\n");
            keep_running = 0;
        }

        if( mod > lmc->code_mod ){
            log_printf(lmc, "Code file is updated.\n");
            lmc->code_mod = mod;
            read_assembler_code(lmc);
        }
        usleep(10E3);
    }

    lmc->shutdown = 1;

    gui_destroy(lmc);
exit1:
    log_destroy(lmc);

exit0:
    fclose(lmc->code_fp);
    return 0;
}
