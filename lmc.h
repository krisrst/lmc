
#ifndef LMC_H
#define LMC_H

#include <pthread.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LINE_MAX_LEN 128

#define OUTPUT_FIELD_WIDTH 30
#define OUTPUT_FIELD_HEIGHT 5

enum lmc_state{
    LMC_HALTED,
    LMC_RUNNING,
    LMC_STEPPING,
    LMC_CODING
};

struct lmc {

    /*
     * The current value of:
     * - accumulator register
     * - program counter
     * - instruction register
     * - address register
     *   */
    int accumulator;
    int pc;
    int ir;
    int ar;

    /*
     * The memory contents
     * */
    int memory[100];

    /*
     * LMC's state
     * */
    enum lmc_state state;

    /*
     * LMC's clock frequency in hertz
     * */
    unsigned int frq;

    /*
     * The pthread structures
     * */
    pthread_t gui;
    pthread_t assembler;

    /*
     * Panels
     * */
    //PANEL * main_pan[107];
    //PANEL * help_pan;
    PANEL * assembler_pan;
    PANEL * machinecode_pan;
    PANEL * acc_pan;
    PANEL * pc_pan;
    PANEL * ir_pan;
    PANEL * ar_pan;
    PANEL * in_pan;
    PANEL * out_pan;
    PANEL * status_pan;
    PANEL * help_pan;
    PANEL * memory_pan[100];

    /*
     * Windows used to output or input data
     * */
    WINDOW * assembler_win;
    WINDOW * machinecode_win;
    WINDOW * acc_win;
    WINDOW * pc_win;
    WINDOW * ir_win;
    WINDOW * ar_win;
    WINDOW * in_win;
    WINDOW * out_win;
    WINDOW * status_win;
    WINDOW * help_win;
    WINDOW * memory_win[100];

    /*
     * Memory that contains the assembler code
     * */
    char assembler_mem[100][LINE_MAX_LEN];

    /*
     * Memory that contains labels
     * */
    char label_mem[100][LINE_MAX_LEN];

    /*
     * Memory containing assembled machine code
     * */
    int machinecode_mem[100];

    /*
     * Number of lines in assembler buffer
     * */
    int nlines;

    /*
     * Scrolling offset
     * */
    int scroll;

    /*
     * Output field
     * */
    int outx;
    char output_mem[OUTPUT_FIELD_HEIGHT][OUTPUT_FIELD_WIDTH];

    /*
     * If 1, the LMC should start shutting down
     * */
    int shutdown;

    /*
     * The version of the LMC
     * */
    int majver;
    int minver;

    /*
     * Log file.
     * */
    FILE * log;

    /*
     * Assembler file.
     * And it's last modified time.
     * */
    int code_fd;
    FILE * code_fp;
    time_t code_mod;
};

#endif // LMC_H
