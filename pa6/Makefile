TARGET = pa6
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = pa6.c seqpair.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemory: $(TARGET)
	$(VALGRIND)=./valgrind.log ./$(TARGET) examples/r8.sp r8.pck

testall: test1 test2 test3 test4 test5 test6

test1: $(TARGET)
	./$(TARGET) examples/r8.sp r8.pck
	diff examples/r8.pck r8.pck

test2: $(TARGET)
	./$(TARGET) examples/r100.sp r100.pck
	diff examples/r100.pck r100.pck

test3: $(TARGET)
	./$(TARGET) examples/r100h.sp r100h.pck
	diff examples/r100h.pck r100h.pck

test4: $(TARGET)
	./$(TARGET) examples/r100v.sp r100v.pck
	diff examples/r100v.pck r100v.pck

test5: $(TARGET)
	./$(TARGET) examples/r1K.sp r1K.pck

test6: $(TARGET)
	./$(TARGET) examples/r2K.sp r2K.pck

clean:
	rm -f $(OBJS) $(TARGET) *.pck *.log	
