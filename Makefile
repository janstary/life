BIN	= life
MAN	= life.1
OBJS	= life.o

CFLAGS	= -Wall -pedantic

PREFIX	= $(HOME)
BINDIR	= $(PREFIX)/bin/
MANDIR	= $(PREFIX)/man/man1

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) $(OBJS)

lint: $(MAN)
	mandoc -Tlint $(MAN)

test: $(BIN)
	./$(BIN)       full-5x5.in
	./$(BIN)      empty-5x5.in
	./$(BIN) -g 1 rigid-5x5.in
	./$(BIN) -g 3  loop-5x5.in
	./$(BIN) -g 1 -b 1  5x5
	./$(BIN) -g 1 -b 4  5x5
	./$(BIN) -g 1 -b 5  5x5
	./$(BIN) -g 1 -b 8  5x5
	./$(BIN) -g 1 -b 9  5x5
	./$(BIN) -g 1 -b 12 5x5
	./$(BIN) -g 1 -b 13 5x5
	./$(BIN) -g 1 -b 16 5x5
	./$(BIN) -g 1 -b 17 5x5
	./$(BIN) -g 1 -b 20 5x5
	./$(BIN) -g 1 -b 21 5x5
	./$(BIN) -g 1 -b 24 5x5
	./$(BIN) -g 1 -b 25 5x5
	./$(BIN) -g 1 -b 28 5x5
	./$(BIN) -g 1 -b 29 5x5
	./$(BIN) -g 1 -b 32 5x5

install: $(BIN) $(MAN) test
	install -d -m 755 $(BINDIR) && install -m 755 $(BIN) $(BINDIR)
	install -d -m 755 $(BINDIR) && install -m 644 $(MAN) $(MANDIR)

uninstall:
	rm -f $(BINDIR)/$(BIN) $(MANDIR)/$(MAN)

clean:
	rm -f $(BIN) $(OBJS)
	rm -f *.core *~
