TARGET = pa1
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)

SRCS = main.c huffman.c

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

#TESTFLAGS = -DTEST_READHEADER -DTEST_DECODE -DTEST_MERGELIST -DDO_NOT_MODIFY

$(TARGET): $(OBJS)
	$(CC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(GCC) $(CFLAGS) $(TESTFLAGS) -c $*.c

testmemory: $(TARGET)
	$(VALGRIND)=./valgrind.log ./$(TARGET) original/lorum lorum.count lorum.tree lorum.code lorum.hbt

testall: test1 test2 test3 test4 test5 test6

test1: $(TARGET)
	./$(TARGET) original/binary1 binary1.count binary1.tree binary1.code binary1.hbt
	diff binary1.count count/binary1.count
	diff binary1.tree tree/binary1.tree
	diff binary1.code code/binary1.code
	diff binary1.hbt compressed/binary1.hbt

test2: $(TARGET)
	./$(TARGET) original/empty empty.count empty.tree empty.code empty.hbt
	diff empty.count count/empty.count
	diff empty.tree tree/empty.tree
	diff empty.code code/empty.code
	diff empty.hbt compressed/empty.hbt

test3: $(TARGET)
	./$(TARGET) original/gophers gophers.count gophers.tree gophers.code gophers.hbt
	diff gophers.count count/gophers.count
	diff gophers.tree tree/gophers.tree
	diff gophers.code code/gophers.code
	diff gophers.hbt compressed/gophers.hbt

test4: $(TARGET)
	./$(TARGET) original/lorum lorum.count lorum.tree lorum.code lorum.hbt
	diff lorum.count count/lorum.count
	diff lorum.tree tree/lorum.tree
	diff lorum.code code/lorum.code
	diff lorum.hbt compressed/lorum.hbt

test5: $(TARGET)
	./$(TARGET) original/stone stone.count stone.tree stone.code stone.hbt
	diff stone.count count/stone.count
	diff stone.tree tree/stone.tree
	diff stone.code code/stone.code
	diff stone.hbt compressed/stone.hbt

test6: $(TARGET)
	./$(TARGET) original/woods woods.count woods.tree woods.code woods.hbt
	diff woods.count count/woods.count
	diff woods.tree tree/woods.tree
	diff woods.code code/woods.code
	diff woods.hbt compressed/woods.hbt

clean:
	rm -f $(OBJS) $(TARGET) *.count *.tree *.code *.hbt *.log *.bin
