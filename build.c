#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int run(char ** args) {
  assert(args && args[0]);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(args[0], args);
    abort();
  } else if (pid > 0) {
    int sl = 0;
    assert(0 <= waitpid(pid, &sl, 0));
    if (WIFEXITED(sl)) return WEXITSTATUS(sl);
  }

  fprintf(stderr, "failed to run child process: %s\n", args[0]);
  return 1;
}

#define CC(...) do { char * argv[] = { __VA_ARGS__, 0 }; if (run(argv)) return 1; } while (0)

int main() {
  CC("clang", "-c", "-o", "treesitter.o", "tree-sitter/lib/src/lib.c", "-I", "tree-sitter/lib/include");
  CC("clang", "-c", "-o", "treesitter-java.o", "tree-sitter-java/src/parser.c", "-I", "tree-sitter/lib/include");

  CC("clang", "-Wall", "-c", "-o", "example.o", "example.c", "-I", "tree-sitter/lib/include");

  CC("clang", "-o", "example", "example.o", "treesitter.o", "treesitter-java.o");

  return 0;
}

