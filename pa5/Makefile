TARGET = pa5
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = pa5.c sorting.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemoryq: $(TARGET)
	$(VALGRIND)=./valgrindq.log ./$(TARGET) -q random.b randomOut.b

testmemorym: $(TARGET)
	$(VALGRIND)=./valgrindm.log ./$(TARGET) -m random.b randomOut.b

testall: test1 test2 test3 test4 test5 test6 test7

qrandom: $(TARGET)
	./generate 100000000 random.b
	./$(TARGET) -q random.b randomOut.b

mrandom: $(TARGET)
	./generate 100000000 random.b
	./$(TARGET) -m random.b randomOut.b

clean:
	rm -f $(OBJS) $(TARGET) *.b *.txt
