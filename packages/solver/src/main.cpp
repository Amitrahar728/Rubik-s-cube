#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "coordcube.h"
#include "cubiecube.h"
#include "enums.h"
#include "search.h"

int main() {
  std::cerr << "Generating solver tables in C++..." << std::endl;
  generateTables();
  std::cerr << "Tables generated successfully." << std::endl;
  std::cout << "READY" << std::endl;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.rfind("SOLVE ", 0) == 0) {
      std::string facelets = line.substr(6);
      // Trim whitespace
      facelets.erase(facelets.find_last_not_of(" \r\n\t") + 1);

      Search searcher;
      std::string sol = searcher.solution(facelets, 22, 10, false);

      if (sol.rfind("Error", 0) == 0) {
        std::cout << "ERROR: " << sol << std::endl;
      } else {
        std::cout << "SOLUTION: " << sol << std::endl;
      }
    } else if (line == "PING") {
      std::cout << "PONG" << std::endl;
    } else if (line == "EXIT") {
      break;
    } else {
      std::cout << "ERROR: Unknown command" << std::endl;
    }
  }
  return 0;
}
