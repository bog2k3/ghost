#include <string>
#include <vector>

std::vector<std::string> strSplit(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  int start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

