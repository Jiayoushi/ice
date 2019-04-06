#include <iostream>

int main(int argc, char *argv[], char *envp[]) {
  std::string body;

  // Variables
  for (char **arg = argv; *arg != nullptr; ++arg) {
    body.append(*arg);
  }
  body.push_back('\n');
  for (char **env = envp; *env != nullptr; ++env) {
    body.append(*env);
  }
  body.push_back('\n');
  // Customized message
  body.append("Hello CGI!\n");

  // Read from stdin and print!
  for (char c; (c = getchar()) != EOF; ) {
    body.push_back(c);
  }

  // Header
  std::string header;
  header.append("HTTP/1.1 200 OK\n");
  header.append("Server: ice\n");
  header.append("Content-Length: " + std::to_string(body.size()) + "\n");
  header.append("Content-Type: text/html\n");
  header.append("\n");

  // Print
  std::cout << header << body;

  return 0;
}
