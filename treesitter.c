#pragma leco add_include_dir "tree-sitter/lib/include"
#pragma leco add_include_dir "tree-sitter/lib/src"

#if __has_include(<tree_sitter/api.h>)
#include "tree-sitter/lib/src/lib.c"
#include "tree-sitter-cpp/src/parser.c"
#include "tree-sitter-cpp/src/scanner.c"
#endif
