TARGET = pa3
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = pa3.c delay.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemory: $(TARGET)
	$(VALGRIND)=./valgrinda.log ./$(TARGET) -a examples/15.b 15sa.b

testall: test1 test2

test1: $(TARGET)
	./$(TARGET) examples/3.txt 3.pre 3.delay
	diff examples/3.pre 3.pre

test2: $(TARGET)
	./$(TARGET) examples/5.txt 5.pre 5.delay
	diff examples/5.pre 5.pre

clean:
	rm -f $(OBJS) $(TARGET) *.delay *.pre *.log
