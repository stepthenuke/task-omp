#include <src/libservice.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

bool checkPath(const std::string& path);
void printFile(const std::string& path);

int main() {
  const std::string serviceName{"org.asdf.catter"};
  const std::vector<std::string> supportedFormats{".txt", ".text", ".info"};

  auto onOpenFile = [&](const std::string& path) {
    if (!checkPath(path)) {
      shr::throwError(serviceName, "OpenFile", "Invalid file.");
    }
    printFile(path);
  };

  shr::SharingService catter{serviceName, supportedFormats,
                             std::move(onOpenFile)};
  catter.start();
}

void printFile(const std::string& path) {
  std::ifstream file{path};
  for (std::string line; std::getline(file, line);) {
    std::cout << line << "\n";
  }
}

bool checkPath(const std::string& path) {
  const fs::path fPath{path};
  return fs::is_regular_file(fPath);
}