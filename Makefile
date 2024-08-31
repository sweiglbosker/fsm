include config.mk

SRC:=$(wildcard $(SRCDIR)/*.c)
OBJ:=$(wildcard $(BINDIR)/*.o)

ifeq ($(suffix $(TARGET)), .a)
	CFLAGS+=-c
endif 
ifeq ($(suffix $(TARGET)), .so)
	CFLAGS+=-c -fPIC
	LDFLAGS+=-shared
endif

debug: CFLAGS+=$(DEBUGFLAGS)
debug: $(TARGET)

release: CFLAGS+=$(RELEASEFLAGS)
release: $(TARGET)

%.o: %.c
	$(CC) -CFLAGS $^

$(TARGET): $(OBJ)
	mkdir $(BINDIR)
        ifeq ($(suffix $(TARGET)), .a)
	        ar -rcs $@ *.o
        else
	        $(CC) -o $@ $(LDFLAGS) $^
        endif

clean:
	rm -rf bin/*


.PHONY=debug release clean
