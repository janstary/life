BIN	= life
MAN	= life.1
OBJS	= life.o

CFLAGS	= -Wall -pedantic

PREFIX	= $(HOME)
BINDIR	= $(PREFIX)/bin/
MANDIR	= $(PREFIX)/man/man1

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) $(OBJS)

lint: $(MAN)
	mandoc -Tlint $(MAN)

test: $(BIN)
	./$(BIN) -G1

install: $(BIN) $(MAN)
	install -d -m 755 $(BINDIR) && install -m 755 $(BIN) $(BINDIR)
	install -d -m 755 $(BINDIR) && install -m 644 $(MAN) $(MANDIR)

uninstall:
	rm -f $(BINDIR)/$(BIN) $(MANDIR)/$(MAN)

clean:
	rm -f $(BIN) $(OBJS)
	rm -f *.core *~
