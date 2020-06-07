TESTS=test00
all: $(TESTS)

test00: test00.c impl.c

.PHONY: check
check: $(TESTS)
	./test00

.PHONY: clean
clean:
	rm -f $(TESTS)
