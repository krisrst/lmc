

#include "cpu.h"
#include "log.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>

/*
 * These functions represent the control logic.
 * */

static int fetch_instruction(struct lmc * lmc){
    return lmc->memory[lmc->pc];
}

static void increment_pc(struct lmc * lmc){
    lmc->pc++;
}

static void decode_instruction(struct lmc * lmc, int instr){

    lmc->ir = instr / 100;
    lmc->ar = instr - (lmc->ir * 100);
}

static int execute_instruction(struct lmc * lmc){

    int temp;
    int c;
    int n;
    char buf[128];
    char * errp;
    int ret = 0;

    if( lmc->ir == 4 ){
        return -1;
    }
    if( lmc->ar > 99 || lmc->ar < 0 )
        return -1;

    switch(lmc->ir){
        // HLT
        case 0:
            lmc->state = LMC_HALTED;
            ret = -1;
            break;

        // ADD
        case 1:
            temp = lmc->accumulator + lmc->memory[lmc->ar];
            if( temp > 999 ) temp = 999;
            if( temp < -999 ) temp = -999;
            lmc->accumulator = temp;
            break;

        // SUB
        case 2:
            temp = lmc->accumulator - lmc->memory[lmc->ar];
            if( temp > 999 ) temp = 999;
            if( temp < -999 ) temp = -999;
            lmc->accumulator = temp;
            break;

        // STA
        case 3:
            lmc->memory[lmc->ar] = lmc->accumulator;
            break;

        // LDA
        case 5:
            lmc->accumulator = lmc->memory[lmc->ar];
            break;

        // BRA
        case 6:
            lmc->pc = lmc->ar;
            break;

        // BRZ
        case 7:
            if( lmc->accumulator == 0 )
                lmc->pc = lmc->ar;
            break;

        // BRP
        case 8:
            if( lmc->accumulator >= 0 )
                lmc->pc = lmc->ar;
            break;

        // I/O
        case 9:
            if( lmc->ar == 1 ){
                top_panel(lmc->in_pan);
                sprintf(buf, "");
                n = 0;
                timeout(-1); // Blocking read
                while(1){

                    mvwprintw(lmc->in_win, 0, 0, "     ");
                    mvwprintw(lmc->in_win, 0, 0, "%s", buf);
                    update_panels();
                    doupdate();

                    c = getch();
                    if( c == '\n' ){
                        buf[n] = '\0';
                        break;
                    }
                    else if( c == KEY_BACKSPACE ){
                        buf[n--] = ' ';
                    }
                    else{
                        buf[n++] = (char) c;
                    }
                }
                temp = strtol( buf, &errp, 10 );
                log_printf(lmc, "temp=%d buf=%s\n", temp, buf);

                // If this triggers, we have to assume that the
                // programmer meant a label, and we'll need to find it
                if( errno || (strlen(errp) != 0) ){
                    log_printf(lmc, "invalid input! (%s)\n", buf);
                }

                lmc->accumulator = temp;

                timeout(0);
            }
            else if( lmc->ar == 2 ){
                sprintf(buf, "%d", lmc->accumulator);
                mvwprintw(lmc->out_win, 0,0, "%s", buf);
            }
            else if( lmc->ar == 3 ){
                sprintf(buf, "%c", lmc->accumulator);
                mvwprintw(lmc->out_win, 0,0, "%s", buf);
            }
            else{
                ret = -1;
                log_printf(lmc, "Invalid AR (%d) for IR=9\n", lmc->ar);
            }
            break;

        default:
            log_printf(lmc, "No such IR: %d\n", lmc->ir);
            ret = -1;
    }

    return ret;
}

int run_execution_loop_once(struct lmc * lmc){

    int next_instruction;

    if( lmc->pc > 99 ){
        lmc->state = LMC_HALTED;
        return -1;
    }

    next_instruction = fetch_instruction( lmc );

    increment_pc( lmc );

    decode_instruction( lmc, next_instruction );

    if( execute_instruction( lmc ) )
        return -1;

    return 0;
}

