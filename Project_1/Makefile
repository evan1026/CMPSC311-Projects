#set to gcc if user has not set compiler within shell environment
CC ?= gcc
CFLAGS ?= -c -Wall -Werror -g -std=gnu99
ODIR = build
SDIR = src

#needed for cse machines, which have glibc version < 2.17
LFLAGS = -lrt

# These next few lines were made with the help of stackoverflow
# They automatically generate build rules for object files and store them in the build folder
# This is just to keep the main directory from becoming cluttered
OBJS = $(ODIR)/list.o $(ODIR)/hashtable.o
TEST_LL_OBJS = $(OBJS) $(ODIR)/test_ll.o
MAIN_OBJS = $(OBJS) $(ODIR)/main.o

all: $(ODIR) $(MAIN_OBJS)
	$(CC) $(LFLAGS) -o wordc $(MAIN_OBJS)

test_ll: $(ODIR) $(TEST_LL_OBJS)
	$(CC) $(LFLAGS) -o test_ll $(TEST_LL_OBJS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(LFLAGS) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o test_ll wordc

$(ODIR):
	mkdir -p $(ODIR)
