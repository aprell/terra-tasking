ifeq ($(CC), $(filter $(CC), cc gcc))
  LDFLAGS += -no-pie
endif

CPPFLAGS = -I../include
CFLAGS += -Wall -Wextra -O3
LDFLAGS += -pthread

GEN_FILES := \
  ../include/terra_tasking_fib.h \
  terra_tasking_fib.c

OBJS := fib.o terra_tasking.o terra_tasking_fib.o

all: fib

fib: $(OBJS) ../lib/libtasking.a

$(GEN_FILES): fib.o

fib.o: fib.t tasking.t

%.o: %.t
	terra $<

test: fib
	@NUM_THREADS=4 ./$< | grep "tasks executed" | sort

clean:
	$(RM) fib $(OBJS) $(GEN_FILES)

.PHONY: all test clean
