# Definições de variáveis de uso em todos os makefiles

FLAGS=-ggdb3 -Wall -Werror

SRCDIR=src
OBJDIR=obj
BINDIR=bin
SUBMITDIR=entregas

HEADERS=$(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/**/*.h)
SRCS=$(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
PROGS=$(wildcard *.c)
BINS=$(patsubst %.c, $(BINDIR)/%, $(PROGS))