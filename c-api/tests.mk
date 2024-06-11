include def.mk

UTEST_DIR=unit-tests
UTEST_BIN_DIR=$(UTEST_DIR)/bin

UTESTS=$(wildcard $(UTEST_DIR)/*.c)
UTESTS_BINS=$(patsubst $(UTEST_DIR)/%.c, $(UTEST_BIN_DIR)/%, $(UTESTS))
UTEST_EXS=$(addprefix test-,$(patsubst $(UTEST_DIR)/T-%.c, %, $(UTESTS)))

CASOSDIR=test-cases

.PHONY: test-all test-* test-cases-trab-int $(CASOSDIR)/*

test-all: $(UTESTS_BINS)

test-%: $(UTEST_BIN_DIR)/T-%
	./$<

f-test-%: $(UTEST_BIN_DIR)/F-%
	./$<

t-valgr-%: $(UTEST_BIN_DIR)/T-%
	valgrind --leak-check=full --show-leak-kinds=all -s ./$<

t-clean:
	rm -r $(UTEST_BIN_DIR)/*

# Compilar um teste unitário
$(UTEST_BIN_DIR)/%: $(UTEST_DIR)/%.c $(OBJS) $(HEADERS)
	gcc $(FLAGS) -o $@ $< $(OBJS)