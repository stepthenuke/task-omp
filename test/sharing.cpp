#include <iostream>
#include <src/libservice.hpp>

int main() {
  std::cout << "sharing\n";

  auto onOpenFile = [&](const std::string& path) {
    std::cout << "Opened file: " << path << std::endl;
  };

  shr::SharingService service{
      "com.asdf.music", {"mp3", "flac", "ogg"}, std::move(onOpenFile)};
  service.start();
}