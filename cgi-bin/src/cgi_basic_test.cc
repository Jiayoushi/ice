#include <iostream>

int main(int argc, char *argv[], char *envp[]) {
  // Print header
  printf("%s", "HTTP/1.1 200 OK\nServer: ice\nContent-Length: 53\nContent-Type: text/html\n\n");

  // Print program
  for (char **arg = argv; *arg != nullptr; ++arg) {
    printf("%s\n", *arg);
  }
  for (char **env = envp; *env != nullptr; ++env) {
    printf("%s\n", *env);
  }
  printf("Hello CGI!\n");
  return 0;
}
