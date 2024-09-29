include config.mk

SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ:=$(SRC:$(SRCDIR)/%.c=$(BINDIR)/%.o)

ifeq ($(suffix $(TARGET)), .a)
	CFLAGS+=-c
endif 
ifeq ($(suffix $(TARGET)), .so)
	CFLAGS+=-c -fPIC
	LDFLAGS+=-shared
endif

default: debug

debug: CFLAGS+=$(DEBUGFLAGS)
debug: $(TARGET)

release: CFLAGS+=$(RELEASEFLAGS)
release: $(TARGET)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	echo hello
	$(CC) -o $@ $(CFLAGS) $^

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET): $(OBJ) $(BINDIR)
	echo $(OBJ)
        ifeq ($(suffix $(TARGET)), .a)
	        ar -rcs $@ $(OBJ)
        else
	        $(CC) -o $@ $(LDFLAGS) $^
        endif

clean:
	rm -rf bin/*

.PHONY=default debug release clean
