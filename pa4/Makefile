TARGET = pa4
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = main.c tree.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemory: $(TARGET)
	$(VALGRIND)=./valgrind.log ./$(TARGET) examples/1000.txt 1000.pre 1000.delay

testall: test1 test2 test3 test4

test1: $(TARGET)
	./$(TARGET) -b examples/ops0.b tree0.b
	diff examples/tree0.b tree0.b
	#diff examples/3.delay 3.delay

test2: $(TARGET)
	./$(TARGET) examples/ops1.b tree1.b
	diff examples/tree1.b tree1.b
	#diff examples/5.pre 5.delay

test3: $(TARGET)
	./$(TARGET) examples/ops2.b tree2.b
	diff examples/tree2.b tree2.b

test4: $(TARGET)
	./$(TARGET) examples/ops3.b tree3.b
	diff examples/tree3.b tree3.b

clean:
	rm -f $(OBJS) $(TARGET) *.delay *.pre *.log
