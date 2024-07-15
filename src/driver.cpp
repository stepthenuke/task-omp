#include <iostream>
#include <src/libservice.hpp>

int main() {
  std::cout << "Registering service started\n";

  shr::RegisteringService regService;
  regService.start();
}