CC = gcc
CPPFLAGS += -I../include
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -pthread

GEN_FILES = \
../include/terra_tasking_fib.h \
terra_tasking_fib.c \

GEN_SRCS = $(filter %.c,$(GEN_FILES))

OBJS = fib.o terra_tasking.o $(GEN_SRCS:.c=.o)

all: fib

fib: $(OBJS) ../lib/libtasking.a

$(GEN_FILES): fib.o

fib.o: fib.t tasking.t

%.o: %.t
	terra $<

clean:
	rm -f fib $(OBJS) $(GEN_FILES)

.PHONY: all clean
