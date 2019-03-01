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
	$(VALGRIND)=./valgrind.log ./$(TARGET) examples/1000.txt 1000.pre 1000.delay

testall: test1 test2 test3 test4 test5

test1: $(TARGET)
	./$(TARGET) examples/3.txt 3.pre 3.delay
	diff examples/3.pre 3.pre
	#diff examples/3.delay 3.delay

test2: $(TARGET)
	./$(TARGET) examples/5.txt 5.pre 5.delay
	diff examples/5.pre 5.pre
	#diff examples/5.pre 5.delay

test3: $(TARGET)
	./$(TARGET) examples/10.txt 10.pre 10.delay

test4: $(TARGET)
	./$(TARGET) examples/100.txt 100.pre 100.delay

test5: $(TARGET)
	./$(TARGET) examples/1000.txt 1000.pre 1000.delay

clean:
	rm -f $(OBJS) $(TARGET) *.delay *.pre *.log
