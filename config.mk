BINDIR = bin
SRCDIR = src
INCDIR = include
TARGET := $(BINDIR)/libfsm.a
STD = c99
CFLAGS := -std=$(STD) -Iinclude -Wall -Wextra -pedantic
LDFLAGS =
DEBUGFLAGS = -g3
RELEASEFLAGS = -Os 
