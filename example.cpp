#pragma leco test
#pragma leco add_impl treesitter

extern "C" {
#include "tree-sitter/lib/include/tree_sitter/api.h"
const TSLanguage *tree_sitter_java();
}

#include <stdio.h>

void dump(TSNode node, unsigned indent) {
  printf("%*s%s %s\n", indent, "", ts_node_grammar_type(node),
         ts_node_string(node));
  for (auto i = 0; i < ts_node_child_count(node); i++) {
    dump(ts_node_child(node, i), indent + 2);
  }
}

int main() {
  TSParser * parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_java());

  TSInput input{
      .payload = fopen("test.java", "rb"),
      .read = [](void *payload, uint32_t index, TSPoint pos,
                 uint32_t *bytes) -> const char * {
        static char buf[10240]{};
        FILE *file = static_cast<FILE *>(payload);
        *bytes = fread(buf, 1, sizeof(buf), file);
        return buf;
      },
      .encoding = TSInputEncodingUTF8,
  };
  TSTree *tree = ts_parser_parse(parser, nullptr, input);
  TSNode node = ts_tree_root_node(tree);
  dump(node, 0);
}
