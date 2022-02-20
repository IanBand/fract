# gcc -g main.c bmp.c complex.c functs.c -lm -fopenmp -std=c99 -o fract
# -DPXPADDING

CC = gcc
DEBUG = -g
DEFINES = 
OPENMP = -lm -fopenmp
STD = -std=c99

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
	-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
	$(DEFINES) $(STD) $(OPENMP)

# the programs that we want to build
PROG1 = main
PROG2 = 
PROG3 = 

PROGS = $(PROG1) $(PROG2) $(PROG3)

# first target is default target, should always be "all" 
all: $(PROGS) #in this example, the default target will build all of the programs

#another target, depends on prog1.o. targets compile command follows its definition
$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $@ $^ 
	chmod a+rx,g-w $@


# $@ is a special var that expands to be the target name
# $^ is a special var that expands to be all the target dependencies (the things to the right of the colon)
# 'chmod a+rx,g-w $@' is just a command that makes the executable "group readable and writable"

#prog1.o is itself a target that needs its own dependencies defined/instructions for how to create/commands needed to update that target
$(PROG1).o: $(PROG1).c
	$(CC) $(CFLAGS) -c $<

	
# $< is a special var that expands to be the first thing in the dependancy list
# -c tells the compileer to compile to object files, but dont link them

# every makefile should have a clean target, removes the object files and executables, basically everything that can be rebuilt
clean: 
	rm -f $(PROGS) *.o *~ \#*

# a target to make a .tar.gz
tar:
	tar --exclude-backups -c -a -f ./${LOGNAME}.tar.gz *.[ch] [mM]akefile
