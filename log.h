
#ifndef LOG_H
#define LOG_H

#include "lmc.h"

int log_init(struct lmc * lmc);
void log_destroy(struct lmc * lmc);

int log_printf(struct lmc * lmc, const char *fmt, ...);

#endif // LOG_H
