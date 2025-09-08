#pragma leco add_impl treesitter.java.c
#pragma leco add_include_dir "tree-sitter/lib/include"
#pragma leco add_include_dir "tree-sitter/lib/src"

#if __has_include(<tree_sitter/api.h>)
#include "tree-sitter/lib/src/lib.c"
#endif
