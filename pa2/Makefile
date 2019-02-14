TARGET = pa2
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = main.c sequence.c shell_array.c shell_list.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemory: $(TARGET)
	$(VALGRIND)=./valgrind.log ./$(TARGET) original/lorum lorum.count lorum.tree lorum.code lorum.hbt

testall: test1 test2 test3 test4 test5

test1: $(TARGET)
	./$(TARGET) -a examples/15.b 15sa.b
	diff examples/15sa.b 15sa.b
	./$(TARGET) -l examples/15.b 15sl.b
	diff examples/15sl.b 15sl.b

test2: $(TARGET)
	./$(TARGET) -a examples/1000.b 1000sa.b
	diff examples/1000sa.b 1000sa.b
	./$(TARGET) -l examples/1000.b 1000sl.b
	diff examples/1000sl.b 1000sl.b

test3: $(TARGET)
	./$(TARGET) -a examples/10000.b 10000sa.b
	./$(TARGET) -l examples/10000.b 10000sl.b

test4: $(TARGET)
	./$(TARGET) -a examples/100000.b 100000sa.b
	./$(TARGET) -l examples/100000.b 100000sl.b

test5: $(TARGET)
	./$(TARGET) -a examples/1000000.b 1000000sa.b
	./$(TARGET) -l examples/1000000.b 1000000sl.b

clean:
	rm -f $(OBJS) $(TARGET) *.b
