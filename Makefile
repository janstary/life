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
	./$(BIN) -G5 -b 10x10
	./$(BIN) -G5 -g 10x10
	./$(BIN) -G5 -c 10x10
	#./$(BIN) life-50x10.in

install: $(BIN) $(MAN)
	install -d -m 755 $(BINDIR) && install -m 755 $(BIN) $(BINDIR)
	install -d -m 755 $(BINDIR) && install -m 644 $(MAN) $(MANDIR)

uninstall:
	rm -f $(BINDIR)/$(BIN) $(MANDIR)/$(MAN)

clean:
	rm -f $(BIN) $(OBJS)
	rm -f *.core *~
