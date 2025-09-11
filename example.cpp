#pragma leco test
#pragma leco add_impl treesitter

extern "C" {
#include "tree-sitter/lib/include/tree_sitter/api.h"
const TSLanguage *tree_sitter_java();
}

import hay;
import jojo;
import print;

void dump(TSNode node, unsigned indent) {
  putfn("%*s%s", indent, "", ts_node_grammar_type(node));
  for (auto i = 0; i < ts_node_child_count(node); i++) {
    dump(ts_node_child(node, i), indent + 2);
  }
}

int main() {
  hay<TSParser *, ts_parser_new, ts_parser_delete> parser {};
  ts_parser_set_language(parser, tree_sitter_java());

  auto src = jojo::read_cstr("test.java");
  TSTree *tree = ts_parser_parse_string(parser, nullptr, src.begin(), src.size());
  TSNode node = ts_tree_root_node(tree);
  dump(node, 0);
}
