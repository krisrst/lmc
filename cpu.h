
#ifndef CPU_H
#define CPU_H

#include "lmc.h"

/*
 * Runs the execution loop, fetching, decoding and
 * executing one single instruction.
 * Returns 0 on success, or -1 on error or halt.
 * */
int run_execution_loop_once(struct lmc * lmc);

#endif // CPU_H
