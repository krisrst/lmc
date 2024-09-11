# LMC
A C implementation of the Little Man Computer (LMC).

This machine simulator is at the moment heavily inspired by Peter Higginson's
version of the LMC. You can find one of those here:

https://peterhigginson.co.uk/lmc

# Why?

This variant of the LMC is for students who are either eager to learn systems
programming in C, or just want yet another way to run their masterpieces of
assembly language code. All programs that run in Peter Higginson's LMC should 
also run in this LMC.

# Prerequisites

You need ncurses if you run this on your own Linux machine. If you have Ubuntu,
you can just install it (from a terminal):

`sudo apt install libncurses5-dev`

# How do I use it?

To use this software, first and foremost you need a terminal. You can usually
get one to pop up by logging in to a Linux machine and press Ctrl+Alt+t. Or you
can search for 'terminal' and launch one from the GUI.

Once you are in a terminal, you must first clone this repository.

`git clone git@github.com:krisrst/lmc.git'

So copy that into your terminal, and press enter.
Then change directory (cd) into the repository:

`cd lmc`

Now you should simply run 'make':

`make`

Which will produce an amount of information:

```
gcc -g -c -o main.o main.c
gcc -g -c -o gui.o gui.c
gcc -g -c -o log.o log.c
gcc -g -c -o cpu.o cpu.c
gcc -g -c -o util.o util.c
gcc -g -c -o assembler.o assembler.c
gcc -g -o lmc main.o gui.o log.o cpu.o util.o assembler.o -lpthread -lncurses -lpanel
```

What you have done now, is to build the LMC. This does the following:

1) Compile source files (.c) to object files (.o)
2) Link the object files into an executable (lmc)

So if you do:

`ls -l`

You will see all the source and header files (.c, .h), object files (.o) and
the produced executable: lmc.

Congratulations, you have built a C program!

# Running the LMC

Running the LMC is simple. You can just do:

`./lmc -h`

To see what the program can do (more or less).

Basically you launch ./lmc with a parameter, which is a file containing
assembly code. An example is provided in the repo (mycode.txt):

`./lmc mycode.txt`

From there, use the built-in help to get going (type '?').

# Contributing

Anyone can and is welcome to contribute to make this project better and more
exciting. I will try to make a list of features I would like to see, but it
is perfectly fine for you to suggest your own. One note though, in this
repository we submit pull requests for newly developed code. Don't worry about
it if you don't know what that is yet. I will make sure to help whoever wants
to understand how to do it.

# Bugs / Features / Getting in touch

Please contact me by email for any of those topics.
krisrst@ifi.uio.no
