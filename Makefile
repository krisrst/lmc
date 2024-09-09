
SRC := main.c
SRC += gui.c
SRC += log.c
SRC += cpu.c
SRC += util.c
SRC += assembler.c

OBJ := $(SRC:.c=.o)

# By default the first rule is run if no target is given to make
all: .depend lmc

.depend: $(SRC)
	rm -f "$@"
	gcc $(CFLAGS) -MM $^ > "$@"

include .depend

%.o: %.c
	gcc -g -c -o $@ $<

lmc: $(OBJ)
	gcc -g -o $@ $(OBJ) -lncurses -lpanel


clean: $(SRC:.c=.o)
	rm -f $^
	rm -f lmc
	rm -f *.d

.PHONY: all clean
