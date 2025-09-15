#pragma leco test
#pragma leco add_impl treesitter

extern "C" {
#include "tree-sitter/lib/include/tree_sitter/api.h"
const TSLanguage *tree_sitter_java();
}

import hay;
import jojo;
import jute;
import print;

void dump(TSNode node, unsigned indent) {
  putfn("%*s%s", indent, "", ts_node_grammar_type(node));
  for (auto i = 0; i < ts_node_child_count(node); i++) {
    dump(ts_node_child(node, i), indent + 2);
  }
}

int main() try {
  auto language = tree_sitter_java();

  hay<TSParser *, ts_parser_new, ts_parser_delete> parser {};
  ts_parser_set_language(parser, language);

  auto src = jojo::read_cstr("test.java");
  TSTree *tree = ts_parser_parse_string(parser, nullptr, src.begin(), src.size());
  TSNode node = ts_tree_root_node(tree);
  //dump(node, 0);

  jute::view query_src = "(import_declaration (scoped_identifier) @import)";
  unsigned err_ofs {};
  TSQueryError err {};
  hay<TSQuery *, ts_query_new, ts_query_delete> query {
    language,
    query_src.begin(),
    query_src.size(),
    &err_ofs,
    &err
  };
  if (err) die("Error: ", static_cast<unsigned>(err));

  hay<TSQueryCursor *, ts_query_cursor_new, ts_query_cursor_delete> cursor {};
  ts_query_cursor_exec(cursor, query, node);

  TSQueryMatch match {};
  while (ts_query_cursor_next_match(cursor, &match)) {
    putln("match");
    for (auto i = 0; i < match.capture_count; i++) {
      dump(match.captures[i].node, 2);
    }
  }
} catch (...) {
  return 1;
}
