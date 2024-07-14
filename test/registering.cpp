#include <iostream>

#include <src/libservice.hpp>

int main() {
   std::cout << "registering\n";

   shr::RegisteringService regService;
   regService.start();
}