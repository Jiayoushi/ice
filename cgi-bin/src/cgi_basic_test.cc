#include <iostream>

int main(int argc, char *argv[], char *envp[]) {
  for (char **arg = argv; *arg != nullptr; ++arg) {
    printf("%s\n", *arg);
  }
  for (char **env = envp; *env != nullptr; ++env) {
    printf("%s\n", *env);
  }
  printf("Hello CGI!\n");
  return 0;
}
