include def.mk

.PHONY: all compile submit-* clean setup

# Compilar todos os executáveis
build: setup $(BINS)

# Gerar os object files (.o) de todos os arquivos .c em src/
compile: setup $(HEADERS) $(OBJS)

# Limpar todos os arquivos binários e .o
clean:
	rm -r $(OBJDIR)/ $(BINDIR)/

# Gerar diretórios necessários para compilação (obj/ e bin/)
setup: $(OBJDIR) $(OBJDIR)/utils $(OBJDIR)/adts $(BINDIR)

$(OBJDIR):
	mkdir $@/

$(OBJDIR)/utils:
	mkdir $@/

$(OBJDIR)/adts:
	mkdir $@/

$(BINDIR):
	mkdir $@/

# Gerar um object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc $(FLAGS) -c -o $@ $<

# Gerar um executável
$(BINDIR)/%: %.c $(OBJS) $(HEADERS)
	gcc $(FLAGS) -o $@ $*.c $(OBJS)

include tests.mk