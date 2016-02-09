CC ?= gcc #set to gcc if user has not set compiler within shell environment
CFLAGS ?= -c -Wall -Werror -g
ODIR = build
SDIR = src

# These next few lines were made with the help of stackoverflow
# They automatically generate build rules for object files and store them in the build folder
# This is just to keep the main directory from becoming cluttered
OBJS = $(ODIR)/word.o $(ODIR)/list.o $(ODIR)/hashtable.o
TEST_LL_OBJS = $(OBJS) $(ODIR)/test_ll.o
MAIN_OBJS = $(OBJS) $(ODIR)/main.o

all: $(ODIR) $(MAIN_OBJS)
	$(CC) -o wordc $(MAIN_OBJS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

test_ll: $(ODIR) $(TEST_LL_OBJS)
	$(CC) -o test_ll $(TEST_LL_OBJS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o test_ll wordc

$(ODIR):
	mkdir -p $(ODIR)
