#include <src/libservice.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

bool checkPath(const std::string &path);
void printFile(const std::string &path);

int main() {
   const std::string serviceName {"me.nesoft.code"};
   const std::vector<std::string> supportedFormats {"c", "cpp", "h", "hpp"};

   auto onOpenFile = [](const std::string &path) { 
      const std::string command = "code " + path;
      std::cout << command << "\n";
      std::system(command.c_str());
   };

   shr::SharingService catter{serviceName, supportedFormats, std::move(onOpenFile)};
   catter.start();
}
