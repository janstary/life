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
	./$(BIN) -g 5 -b 1 5x5
	./$(BIN) -g 5 -b 2 5x5
	./$(BIN) -g 5 -b 3 5x5
	./$(BIN) -g 5 -b 4 5x5
	./$(BIN) -g 5 -b 5 5x5
	./$(BIN) -g 5 -b 6 5x5
	./$(BIN) -g 5 -b 7 5x5
	./$(BIN) -g 5 -b 8 5x5

install: $(BIN) $(MAN) test
	install -d -m 755 $(BINDIR) && install -m 755 $(BIN) $(BINDIR)
	install -d -m 755 $(BINDIR) && install -m 644 $(MAN) $(MANDIR)

uninstall:
	rm -f $(BINDIR)/$(BIN) $(MANDIR)/$(MAN)

clean:
	rm -f $(BIN) $(OBJS)
	rm -f *.core *~
