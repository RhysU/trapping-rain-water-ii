TESTS=test00 test01 test02 test03 test04 test05 test06 test07
all: $(TESTS)

test00: test00.c impl.c
test01: test01.c impl.c
test02: test02.c impl.c
test03: test03.c impl.c
test04: test04.c impl.c
test05: test05.c impl.c
test06: test06.c impl.c
test07: test07.c impl.c

.PHONY: check
check: $(TESTS)
	./test00
	./test01
	./test02
	./test03
	./test04
	./test05
	./test06
	./test07

.PHONY: clean
clean:
	rm -f $(TESTS)
