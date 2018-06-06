#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

AR = ar

# define any compile-time flags
skin = alchemy
CFLAGS := $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --cflags)
CFLAGS += -g -O3 -ffunction-sections

# define any directories containing header files other than /usr/include
#
INCLUDES += -I./

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LDLIBS += $(shell /usr/xenomai/bin/xeno-config --skin=$(skin) --ldflags)

AFLAGS = cr

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS =-lnative
LIBS +=-lxenomai
LIBS +=-lrtdm

# define the C source files
SRCS = osal.c

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
MAIN = osal

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)

$(MAIN): $(OBJS)
	$(AR) $(AFLAGS) ../lib/lib$(MAIN).a $(OBJS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ ../lib/lib$(MAIN).a


# DO NOT DELETE THIS LINE -- make depend needs it
