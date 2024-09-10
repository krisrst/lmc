

#include "gui.h"
#include "log.h"
#include "cpu.h"
#include "assembler.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

static void msleep(int ms){
    usleep(ms*1000);
}

static void draw_frame(int x, int y, int w, int h,
        char hor, char ver, char corner, char * name){
    int j, i;

    // Vertical
    for(j=y; j < (y+h); j++){
        mvaddch(j,x,ver);
        mvaddch(j,x+w,ver);
    }

    // Horizontal
    for(i=x; i < (x+w); i++){
        mvaddch(y,i,hor);
        mvaddch(y+h,i,hor);
    }

    // Corners
    mvaddch(y, x, corner);
    mvaddch(y+h, x, corner);
    mvaddch(y, x+w, corner);
    mvaddch(y+h, x+w, corner);

    if( name != NULL){
        assert(strlen(name) < (w-2));

        i = w/2 - strlen(name)/2 + x;
        for(j=0; j < strlen(name); j++){
            mvaddch(y, i+j, name[j]);
        }
    }
}

static void init_background(struct lmc * lmc){

    int i, j;

    lmc->help_win = newwin(GUI_HELP_FIELD_HEIGHT-1,
            GUI_HELP_FIELD_WIDTH-1,
            GUI_HELP_FIELD_START_Y+1,
            GUI_HELP_FIELD_START_X+1);
    box(lmc->help_win, '|', '-');
    lmc->help_pan = new_panel( lmc->help_win );
    hide_panel(lmc->help_pan);

#if 0
    draw_frame(GUI_ASSEMBLY_FIELD_START_X, GUI_ASSEMBLY_FIELD_START_Y,
            GUI_ASSEMBLY_FIELD_WIDTH, GUI_ASSEMBLY_FIELD_HEIGHT,
            '-', '|', '*', "");
    draw_frame(GUI_MACHINECODE_FIELD_START_X, GUI_MACHINECODE_FIELD_START_Y,
            GUI_MACHINECODE_FIELD_WIDTH, GUI_MACHINECODE_FIELD_HEIGHT,
            '-', '|', '*', "");

    draw_frame(GUI_OUTPUT_FIELD_START_X, GUI_OUTPUT_FIELD_START_Y,
            GUI_OUTPUT_FIELD_WIDTH, GUI_OUTPUT_FIELD_HEIGHT,
            '-', '|', '*', "OUTPUT");
#endif

    draw_frame(GUI_CPU_FIELD_START_X, GUI_CPU_FIELD_START_Y,
            GUI_CPU_FIELD_WIDTH, GUI_CPU_FIELD_HEIGHT,
            '-', '|', '*', "CPU");

    draw_frame(GUI_PC_OFF_X, GUI_PC_OFF_Y,
            GUI_PC_WIDTH, GUI_PC_HEIGHT,
            '-', '|', '*', "Program Counter");

    draw_frame(GUI_IR_OFF_X, GUI_IR_OFF_Y,
            GUI_IR_WIDTH, GUI_IR_HEIGHT,
            '-', '|', '*', "Instruction Register");

    draw_frame(GUI_AR_OFF_X, GUI_AR_OFF_Y,
            GUI_AR_WIDTH, GUI_AR_HEIGHT,
            '-', '|', '*', "Address Register");

    draw_frame(GUI_ACC_OFF_X, GUI_ACC_OFF_Y,
            GUI_ACC_WIDTH, GUI_ACC_HEIGHT,
            '-', '|', '*', "Accumulator");

    draw_frame(GUI_ALU_FIELD_START_X, GUI_ALU_FIELD_START_Y,
            GUI_ALU_FIELD_WIDTH, GUI_ALU_FIELD_HEIGHT,
            '-', '|', '*', "ALU");

    draw_frame(GUI_INPUT_FIELD_START_X, GUI_INPUT_FIELD_START_Y,
            GUI_INPUT_FIELD_WIDTH, GUI_INPUT_FIELD_HEIGHT,
            '-', '|', '*', "INPUT");

    draw_frame(GUI_MEMORY_FIELD_START_X, GUI_MEMORY_FIELD_START_Y,
            GUI_MEMORY_FIELD_WIDTH, GUI_MEMORY_FIELD_HEIGHT,
            '-', '|', '*', "MEMORY");

    for(j=0; j < 10; j++){
        for(i=0; i < 10; i++){

            char buf[10];
            sprintf(buf, "%u", i+j*10);


            draw_frame(GUI_MEMORY_FIELD_START_X + GUI_CELL_OFF_X + i*GUI_CELL_WIDTH,
                    GUI_MEMORY_FIELD_START_Y + GUI_CELL_OFF_Y + j*GUI_CELL_HEIGHT,
                    GUI_CELL_OFF_X, GUI_CELL_OFF_Y, '-', '|', '*', buf);
//                    "%u", i + j*10);

            lmc->memory_win[i+j*10] = newwin( 1, 5, GUI_MEMORY_FIELD_START_Y + 3 + j*GUI_CELL_HEIGHT,
                    GUI_MEMORY_FIELD_START_X + 7 + i*GUI_CELL_WIDTH);
            lmc->memory_pan[i+j*10] = new_panel( lmc->memory_win[i+j*10] );
        }
    }

    lmc->assembler_win = newwin(GUI_ASSEMBLY_FIELD_HEIGHT,
            GUI_ASSEMBLY_FIELD_WIDTH,
            GUI_ASSEMBLY_FIELD_START_Y,
            GUI_ASSEMBLY_FIELD_START_X);
    box(lmc->assembler_win, '|', '-');
    lmc->assembler_pan = new_panel( lmc->assembler_win );

    lmc->machinecode_win = newwin(GUI_MACHINECODE_FIELD_HEIGHT,
            GUI_MACHINECODE_FIELD_WIDTH,
            GUI_MACHINECODE_FIELD_START_Y,
            GUI_MACHINECODE_FIELD_START_X);
    box(lmc->machinecode_win, '|', '-');
    lmc->machinecode_pan = new_panel( lmc->machinecode_win );

    lmc->pc_win = newwin(1, 3,
            GUI_PC_OFF_Y + 1, GUI_PC_OFF_X + GUI_PC_WIDTH/2);
    lmc->pc_pan = new_panel( lmc->pc_win );

    lmc->ir_win = newwin(1, 3,
            GUI_IR_OFF_Y + 1, GUI_IR_OFF_X + GUI_IR_WIDTH/2);
    lmc->ir_pan = new_panel( lmc->ir_win );

    lmc->ar_win = newwin(1, 3,
            GUI_AR_OFF_Y + 1, GUI_AR_OFF_X + GUI_AR_WIDTH/2);
    lmc->ar_pan = new_panel( lmc->ar_win );

    lmc->acc_win = newwin(1, 5,
            GUI_ACC_OFF_Y + 1, GUI_ACC_OFF_X + GUI_ACC_WIDTH/2);
    lmc->acc_pan = new_panel( lmc->acc_win );

    lmc->out_win = newwin(GUI_OUTPUT_FIELD_HEIGHT,
            GUI_OUTPUT_FIELD_WIDTH,
            GUI_OUTPUT_FIELD_START_Y,
            GUI_OUTPUT_FIELD_START_X);
    box(lmc->out_win, '|', '-');
    lmc->out_pan = new_panel( lmc->out_win );

    lmc->in_win = newwin(1, 5, GUI_INPUT_FIELD_START_Y+1,
            GUI_INPUT_FIELD_START_X + 1);
    lmc->in_pan = new_panel( lmc->in_win );

#if 0
    draw_frame(GUI_STATUS_FIELD_START_X, GUI_STATUS_FIELD_START_Y,
            GUI_STATUS_FIELD_WIDTH, GUI_STATUS_FIELD_HEIGHT,
            '-', '|', '*', "STATUS");
#endif
    lmc->status_win = newwin(GUI_STATUS_FIELD_HEIGHT,
            GUI_STATUS_FIELD_WIDTH,
            GUI_STATUS_FIELD_START_Y,
            GUI_STATUS_FIELD_START_X);
    box(lmc->status_win, '|', '-');
    lmc->status_pan = new_panel( lmc->status_win );

    update_panels();

}

void status_field_print(struct lmc * lmc, const char *fmt, ...){

    int length, n, i;
    char buf[STATUS_FIELD_WIDTH];
    va_list va;

    for(n=STATUS_FIELD_HEIGHT-1; n >= 1; n--){
        memcpy(lmc->status_mem[n], lmc->status_mem[n-1], STATUS_FIELD_WIDTH);
    }

    va_start(va, fmt);

    length = vsnprintf(buf, STATUS_FIELD_WIDTH, fmt, va);
    va_end(va);

    for(n=length; n < STATUS_FIELD_WIDTH; n++){
        buf[n] = ' ';
    }
    buf[STATUS_FIELD_WIDTH-1] = '\0';

    snprintf(lmc->status_mem[0], STATUS_FIELD_WIDTH, "%s", buf);

    // Status field
    for(i=0; i < STATUS_FIELD_HEIGHT; i++){
        mvwprintw(lmc->status_win, 1+i, 1, "%s", lmc->status_mem[i]);
    }
}

static void print_header(struct lmc * lmc){
    mvprintw( 1, 1, "Assembly Language Code" );
}

static void* assembler_thread(void * opaque){
    struct lmc * lmc;

    lmc = (struct lmc *) opaque;

    while(!lmc->shutdown){
        msleep(10);
    }

    return NULL;
}

static int within_region(int x, int y, int w, int h, int mx, int my){

    if( (mx >= x) && (mx < (x+w)) &&
        (my >= y) && (my < (y+h)) )
        return 1;

    return 0;
}

static void read_to_window(struct lmc * lmc, WINDOW * win, PANEL * pan,
        int * current, int min, int max, char * clear){

    int n, c;
    int temp;
    char * errp;
    char buf[128];

    top_panel(pan);
    sprintf(buf, "%s", clear);
    n = 0;
    timeout(-1); // Blocking read
    while(1){

        mvwprintw(win, 0, 0, "%s", clear);
        mvwprintw(win, 0, 0, "%s", buf);
        update_panels();
        doupdate();
        log_printf(lmc, "test22\n");

        c = getch();
        if( c == '\n' ){
            buf[n] = '\0';
            break;
        }
        else if( c == KEY_MOUSE ){}
        else if( c == KEY_BACKSPACE ){
            buf[n--] = ' ';
            if(n < 0) n = 0;
            buf[n] = '\0';
        }
        else{
            buf[n++] = (char) c;
            buf[n] = '\0';
        }
    }
    timeout(0);
    temp = strtol( buf, &errp, 10 );
    log_printf(lmc, "temp=%d buf=%s\n", temp, buf);

    // If this triggers, we have to assume that the
    // programmer meant a label, and we'll need to find it
    if( errno || (strlen(errp) != 0) ){
        log_printf(lmc, "invalid input! (%s)\n", buf);
        mvwprintw(win, 0, 0, "%s", clear);
        return;
    }

    if( temp < min || temp > max ){
        mvwprintw(win, 0, 0, "%s", clear);
    }
    else
        *current = temp;
}

static void select_window(struct lmc * lmc, int x, int y){

    if( within_region(GUI_ASSEMBLY_FIELD_START_X, GUI_ASSEMBLY_FIELD_START_Y,
                GUI_ASSEMBLY_FIELD_WIDTH, GUI_ASSEMBLY_FIELD_HEIGHT,
                x, y)){

        top_panel(lmc->assembler_pan);
        log_printf(lmc, "off y %d off x %d\n", y - GUI_ASSEMBLY_FIELD_START_Y,
                x - GUI_ASSEMBLY_FIELD_START_X);
    }

    if( within_region(GUI_MEMORY_FIELD_START_X, GUI_MEMORY_FIELD_START_Y,
                GUI_MEMORY_FIELD_WIDTH, GUI_MEMORY_FIELD_HEIGHT,
                x, y)){

        int cell;
        int cx, cy;

        cx = x - GUI_MEMORY_FIELD_START_X;
        cy = y - GUI_MEMORY_FIELD_START_Y;
        assert(cx >= 0);
        assert(cy >= 0);
        assert(cx < (GUI_MEMORY_FIELD_START_X + GUI_MEMORY_FIELD_WIDTH));
        assert(cy < (GUI_MEMORY_FIELD_START_Y + GUI_MEMORY_FIELD_HEIGHT));

        cx -= GUI_CELL_OFF_X;
        cy -= GUI_CELL_OFF_Y;

        cx /= GUI_CELL_WIDTH;
        cy /= GUI_CELL_HEIGHT;

        //log_printf(lmc, "cell %d %d (%d)\n", cx, cy, cx + cy*10);
        cell = cx + cy * 10;

        assert(cell >= 0);
        assert(cell < 100);

        read_to_window( lmc, lmc->memory_win[ cell ], lmc->memory_pan[ cell ],
                &lmc->memory[ cell ], -999, 999, "     ");
    }
}

static void refresh_all(struct lmc * lmc){
    int i;

    wrefresh(lmc->pc_win);
    wrefresh(lmc->ir_win);
    wrefresh(lmc->ar_win);
    wrefresh( lmc->assembler_win );
    for(i=0; i < 100; i++){
         wrefresh( lmc->memory_win[i] );
    }

}

static void update_window_contents(struct lmc * lmc){

    int i;
    char buf[128];

    // Assemblerfield
    for(i=0; i < GUI_ASSEMBLY_FIELD_WIDTH - 2; i++)
        buf[i] = ' ';
    buf[GUI_ASSEMBLY_FIELD_WIDTH - 2] = '\0';

    for(i=0; i < 50; i++)
        mvwprintw(lmc->assembler_win, 1+i,1, "%s", buf);

    for(i=0; i < 50; i++){
        if( i >= lmc->nlines ) break;
        mvwprintw(lmc->assembler_win, 1+i,1, "%s", lmc->assembler_mem[i+lmc->scroll]);
    }

    // Machinecode field
    for(i=0; i < GUI_MACHINECODE_FIELD_WIDTH - 2; i++)
        buf[i] = ' ';
    buf[GUI_MACHINECODE_FIELD_WIDTH - 2] = '\0';

    for(i=0; i < 50; i++)
        mvwprintw(lmc->machinecode_win, 1+i,1, "%s", buf);

    for(i=0; i < 50; i++){
        if( i >= (lmc->nlines - lmc->scroll)) break;
        mvwprintw(lmc->machinecode_win, 1+i,1, "%d", lmc->machinecode_mem[i+lmc->scroll]);
    }

    for(i=0; i < OUTPUT_FIELD_HEIGHT; i++){
        mvwprintw(lmc->out_win, 1+i, 1, "%s", lmc->output_mem[i]);
    }

    sprintf(buf, "%d", lmc->pc);
    mvwprintw(lmc->pc_win, 0,0, "%s", (const char*) buf);

    sprintf(buf, "%d", lmc->ir);
    mvwprintw(lmc->ir_win, 0,0, "%s", (const char*) buf);

    sprintf(buf, "%d", lmc->ar);
    mvwprintw(lmc->ar_win, 0,0, "%s", (const char*) buf);

    sprintf(buf, "%d", lmc->accumulator);
    mvwprintw(lmc->acc_win, 0,0, "%s", (const char*) buf);

    for(i=0; i < 100; i++){
        sprintf(buf+1, "%0d\n", lmc->memory[i]);
        int d0, d1, d2;
        int val = abs(lmc->memory[i]);

        d0 = val / 100;
        val -= (d0 * 100);
        d1 = val / 10;
        val -= (d1 * 10);
        d2 = val;

        sprintf(buf+1, "%0d", d0);
        sprintf(buf+2, "%0d", d1);
        sprintf(buf+3, "%0d", d2);

        if( lmc->memory[i] < 0 )
            buf[0] = '-';
        else
            buf[0] = ' ';
        mvwprintw(lmc->memory_win[i], 0, 0, "     ");
        mvwprintw(lmc->memory_win[i], 0, 0, "%s", buf);
    }
}

static void* gui_thread(void * opaque){

    int c, ret, i;
    MEVENT event;
    struct lmc * lmc;

    lmc = (struct lmc *) opaque;

    status_field_print(lmc, "Press '?' for help.");

    while(!lmc->shutdown){

        update_window_contents(lmc);

        update_panels();
        doupdate();

        c = getch();

        if( lmc->state == LMC_RUNNING ){
            ret = run_execution_loop_once(lmc);
            if(ret){
                status_field_print(lmc, "Halted!");
                lmc->state = LMC_HALTED;
            }
        }

        if( c == KEY_MOUSE ){
            if( getmouse(&event) == OK ){

                if( event.bstate == BUTTON1_CLICKED ){
                    log_printf(lmc, "BUTTON1_CLICKED %d %d\n", event.x, event.y);
                    select_window( lmc, event.x, event.y);
                }
                else{
                    log_printf(lmc, "Unknown mouse event: %d %d %d\n", event.bstate, event.x, event.y);
                }
            }
        }
        else if( c >= 0 ){
            if( c == '?' ){
                mvwprintw( lmc->help_win, 1,2,  "Help topics for the LMC");
                mvwprintw( lmc->help_win, 2,2,  "CTRL+c:     Close the LMC.");
                mvwprintw( lmc->help_win, 3,2,  "Arrow-up:   Scroll up.");
                mvwprintw( lmc->help_win, 4,2,  "Arrow-down: Scroll down.");
                mvwprintw( lmc->help_win, 5,2,  "r:          Run LMC in normal mode.");
                mvwprintw( lmc->help_win, 6,2,  "s:          Run LMC in stepping mode.");
                mvwprintw( lmc->help_win, 7,2,  "            Press this key again to advance one instruction.");
                mvwprintw( lmc->help_win, 8,2,  "a:          Run the assembler and program the memory.");
                mvwprintw( lmc->help_win, 9,2,  "p:          Reset LMC.");
                mvwprintw( lmc->help_win, 10,2, "?:          Open help.");
                mvwprintw( lmc->help_win, 11,2, "q:          Close help");
                top_panel(lmc->help_pan);
                show_panel(lmc->help_pan);
            }
            else if( c == 'q'){
                lmc->state = LMC_CODING;
                hide_panel(lmc->help_pan);
                top_panel(lmc->assembler_pan);
            }
            else if( (lmc->state == LMC_CODING) && (c == 'a') ){
                ret = assemble( lmc );
                if( ret ){
                    status_field_print(lmc, "Assembler failure!");
                }
                else{
                    status_field_print(lmc, "Assembler success!");
                    // Update memory contents
                    for(i=0; i < 100; i++)
                        lmc->memory[i] = lmc->machinecode_mem[i];
                }
            }
            else if( (lmc->state == LMC_CODING) && c == 's'){
                status_field_print(lmc, "Stepping! 's' to step.");
                lmc->state = LMC_STEPPING;
            }
            else if( (lmc->state == LMC_CODING) && c == 'r'){
                status_field_print(lmc, "Running!");
                lmc->state = LMC_RUNNING;
            }
            else if( (lmc->state == LMC_CODING) && c == KEY_UP){
                lmc->scroll++;
                if(lmc->scroll > (100 - GUI_ASSEMBLY_FIELD_HEIGHT - 2))
                    lmc->scroll = 100 - GUI_ASSEMBLY_FIELD_HEIGHT - 2;
            }
            else if( (lmc->state == LMC_CODING) && c == KEY_DOWN){
                lmc->scroll--;
                if(lmc->scroll < 0) lmc->scroll = 0;
            }
            else if( (lmc->state == LMC_STEPPING) && c == 's'){
                ret = run_execution_loop_once(lmc);
                if(ret){
                    status_field_print(lmc, "Halted!");
                    lmc->state = LMC_HALTED;
                }
            }
            else if( ((lmc->state == LMC_HALTED) || (lmc->state == LMC_CODING)) &&
                    c == 'p'){
                lmc->state = LMC_CODING;
                lmc->accumulator = 0;
                lmc->pc = 0;
                lmc->ar = 0;
                lmc->ir = 0;
                lmc->outx = 0;
                for(i=0; i < 100; i++) lmc->memory[i] = 0;
                for(i=0; i < 100; i++) lmc->machinecode_mem [i] = 0;
                for(i=0; i < OUTPUT_FIELD_HEIGHT; i++){
                    memset(lmc->output_mem[i], 32, OUTPUT_FIELD_WIDTH);
                    lmc->output_mem[i][OUTPUT_FIELD_WIDTH-1] = '\0';
                }
                status_field_print(lmc, "LMC has been reset!");
                status_field_print(lmc, "Press '?' for help.");
            }
            else{
                log_printf(lmc, "Got key: %c (%s)\n", (char) c, unctrl(c));
            }
        }
        msleep(10);
        //log_printf(lmc, "c=%d\n", c);
    }

    return NULL;
}

int gui_init(struct lmc * lmc){

    int ret;
    mmask_t dummy;

    initscr();

    noecho();

    timeout(0);

    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);

    init_background(lmc);
    print_header(lmc);

    refresh();

    ret = pthread_create(&lmc->gui, NULL, gui_thread, (void*) lmc);
    if(ret){
        fprintf(stderr, "gui_thread: pthread_create ret=%d\n", ret);
        return -1;
    }

    ret = pthread_create(&lmc->assembler, NULL, assembler_thread, (void*) lmc);
    if(ret){
        fprintf(stderr, "gui_thread: pthread_create ret=%d\n", ret);
        return -1;
    }

    return 0;
}

void gui_destroy(struct lmc * lmc){

    void * retv;
    pthread_join(lmc->gui, &retv);
    pthread_join(lmc->assembler, &retv);

    endwin();
}
