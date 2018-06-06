#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
skin = alchemy
CFLAGS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --cflags)
LDLIBS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --ldflags)

#SOEM EtherCAT
CFLAGS += -I../../soem 
CFLAGS += -I../../osal
CFLAGS += -I../../oshw
CFLAGS += -O3 -Wall -g


# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:


#SOEM EtherCAT
LDLIBS += -L../../lib
LDLIBS +=-lsoem
LDLIBS +=-losal
LDLIBS +=-loshw
LDLIBS +=-lwiznet_drv

# define the C source files
SRCS = main.c

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)
 
# define the executable file 
MAIN = xmc4800

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LDLIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(LDLIBS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)


# DO NOT DELETE THIS LINE -- make depend needs it