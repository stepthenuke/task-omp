#include "registering_service.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace shr {

namespace detail {

std::optional<std::string> FormatServiceDict::findService(const std::string &format) {
   auto it = data_.find(format);
   if (it == data_.end()) {
      return std::nullopt;
   }
   // formally, it's a random serice
   return { it->second.front() };
}

void FormatServiceDict::pushService(const std::string &serviceName, const std::vector<std::string> &supportedFormats) {
   std::for_each(
      supportedFormats.begin(),
      supportedFormats.end(),
      [&] (auto &&format) {
         data_["." + format].push_back(serviceName);
      }
   );
}

} // namespace detail

void RegisteringService::start() {
   auto [connection, object] = detail::setupSessionConnection(name_, objectPath_);
   
   auto registerService = [this] (const std::string &name, const std::vector<std::string> &supportedFormats) {
      return this->registerServiceHandler(name, supportedFormats);
   };
   auto openFile = [this] (const std::string &path) {
      return this->openFileHandler(path);
   };
   auto openFileUsingService = [this] (const std::string &path, const std::string &service) {
      return this->openFileUsingServiceHandler(path, service);
   };

   object->addVTable(
      sdbus::registerMethod("RegisterService").implementedAs(std::move(registerService)),
      sdbus::registerMethod("OpenFile").implementedAs(std::move(openFile)),
      sdbus::registerMethod("OpenFileUsingService").implementedAs(std::move(openFileUsingService))
   ).forInterface(name_);

   connection->enterEventLoop();
}

void RegisteringService::registerServiceHandler(const std::string &name, const std::vector<std::string> &supportedFormats) {
   serviceDict_.pushService(name, supportedFormats);
}

void RegisteringService::openFileHandler(const std::string &path) {
   fs::path fPath{path};
   auto format = fPath.extension().string();
   auto serviceName = serviceDict_.findService(format);
   if (!serviceName.has_value()) {
      return;
   }
   openFileUsingServiceHandler(path, *serviceName);
}

void RegisteringService::openFileUsingServiceHandler(const std::string &path, const std::string &service) {
   auto serviceProxy = sdbus::createProxy(sdbus::ServiceName(service), sdbus::ObjectPath("/"));
   sdbus::InterfaceName interface{service};
   {
      serviceProxy->callMethod("OpenFile").onInterface(interface).withArguments(path);
   }
}

} // namespace shr