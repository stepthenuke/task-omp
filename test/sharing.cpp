#include <src/libservice.hpp>

#include <iostream>

int main() {
   std::cout << "sharing\n";

   const auto name = "com.asdf.music";
   auto onOpenFile = [&] (const std::string &path) {
      std::cout << "Opened file: " << path << std::endl;
   };

   shr::SharingService service{"com.asdf.music", {"mp3", "flac", "ogg"}, std::move(onOpenFile)};
   service.start();
}