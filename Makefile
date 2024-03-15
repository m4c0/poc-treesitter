all: example
	./example test.cppm
	
example: tree-sitter/libtree-sitter.a parser.o scanner.o example.o

example.o: example.cpp
	clang -std=c++20 -c $< -o $@

tree-sitter/libtree-sitter.a:
	make -C tree-sitter

parser.o: tree-sitter-cpp/src/parser.c
	clang -o $@ -c $<

scanner.o: tree-sitter-cpp/src/scanner.c
	clang -o $@ -c $<
