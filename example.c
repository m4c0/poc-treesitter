#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "tree_sitter/api.h"

const TSLanguage * tree_sitter_java();

static char * slurp(const char * file) {
  FILE * f = fopen(file, "rb");
  assert(f);

  assert(0 == fseek(f, 0, SEEK_END));
  long sz = ftell(f);
  assert(sz);
  assert(0 == fseek(f, 0, SEEK_SET));

  char * data = malloc(sz + 1);
  assert(1 == fread(data, sz, 1, f));
  data[sz] = 0;

  fclose(f);
  return data;
}

static TSQuery * query(TSTree * tree, const char * src) {
  unsigned err_ofs;
  TSQueryError err;
  TSQuery * query = ts_query_new(ts_tree_language(tree), src, strlen(src), &err_ofs, &err);
  assert(!err);
  return query;
}

int main() {
  TSParser * p = ts_parser_new();
  ts_parser_set_language(p, tree_sitter_java());

  const char * src = slurp("test.java");

  TSTree * tree = ts_parser_parse_string(p, NULL, src, strlen(src));

  TSQuery * q = query(
      tree,
      "(import_declaration (scoped_identifier name: (identifier) @short-name) @fqn)");

  TSQueryCursor * cur = ts_query_cursor_new();

  TSQueryMatch m;

  ts_query_cursor_exec(cur, q, ts_tree_root_node(tree));
  while (ts_query_cursor_next_match(cur, &m)) {
    printf("imports by match\n");
    for (int i = 0; i < m.capture_count; i++) {
      const TSNode n = m.captures[i].node;
      int s = ts_node_start_byte(n);
      int e = ts_node_end_byte(n);
      printf("  %.*s\n", e - s, src + s);
    }
  }

  printf("\nimports by capture\n");
  unsigned cap;
  ts_query_cursor_exec(cur, q, ts_tree_root_node(tree));
  while (ts_query_cursor_next_capture(cur, &m, &cap)) {
    const TSNode n = m.captures[cap].node;
    int s = ts_node_start_byte(n);
    int e = ts_node_end_byte(n);
    printf("  %.*s\n", e - s, src + s);
  }

  ts_query_cursor_delete(cur);
  ts_query_delete(q);
  ts_parser_delete(p);
}

