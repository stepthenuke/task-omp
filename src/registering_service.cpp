#include "registering_service.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace shr {

namespace detail {

std::optional<std::string> FormatServiceDict::findService(
    const std::string& format) {
  auto it = formatServiceMap_.find(format);
  if (it == formatServiceMap_.end()) {
    return std::nullopt;
  }
  // formally, it's a random service
  return {it->second.front()};
}

void FormatServiceDict::addService(
    const std::string& serviceName,
    const std::vector<std::string>& supportedFormats) {
  serviceFormatMap_.emplace(serviceName, supportedFormats);
  std::for_each(supportedFormats.begin(), supportedFormats.end(),
                [&](auto&& format) {
                  formatServiceMap_[format].push_back(serviceName);
                });
}

bool FormatServiceDict::removeService(const std::string& serviceName) {
  auto it = serviceFormatMap_.find(serviceName);
  if (it == serviceFormatMap_.end()) {
    return false;
  }
  auto& formatsToDelete = it->second;
  std::for_each(formatsToDelete.begin(), formatsToDelete.end(),
                [this, serviceName](auto&& format) {
                  this->removeServiceFromFormatMap(format, serviceName);
                });
  serviceFormatMap_.erase(it);
  return true;
}

void FormatServiceDict::removeServiceFromFormatMap(
    const std::string& format, const std::string& serviceName) {
  std::deque<std::string>& servicesToRemoveFrom = formatServiceMap_[format];
  servicesToRemoveFrom.erase(
      std::remove(servicesToRemoveFrom.begin(), servicesToRemoveFrom.end(),
                  serviceName),
      servicesToRemoveFrom.end());
  if (formatServiceMap_[format].size() == 0) {
    formatServiceMap_.erase(format);
  }
}

}  // namespace detail

void RegisteringService::start() {
  auto [connection, object] =
      detail::setupSessionConnection(name_, objectPath_);

  auto registerService = [this](
                             const std::string& name,
                             const std::vector<std::string>& supportedFormats) {
    return this->registerServiceHandler(name, supportedFormats);
  };
  auto openFile = [this](const std::string& path) {
    return this->openFileHandler(path);
  };
  auto openFileUsingService = [this](const std::string& path,
                                     const std::string& service) {
    return this->openFileUsingServiceHandler(path, service);
  };

  object
      ->addVTable(sdbus::registerMethod("RegisterService")
                      .withInputParamNames("name", "supportedFormats")
                      .implementedAs(std::move(registerService)),
                  sdbus::registerMethod("OpenFile")
                      .withInputParamNames("path")
                      .implementedAs(std::move(openFile)),
                  sdbus::registerMethod("OpenFileUsingService")
                      .withInputParamNames("path", "service")
                      .implementedAs(std::move(openFileUsingService)))
      .forInterface(name_);

  connection->enterEventLoop();
}

void RegisteringService::registerServiceHandler(
    const std::string& name, const std::vector<std::string>& supportedFormats) {
  serviceDict_.addService(name, supportedFormats);
}

void RegisteringService::openFileHandler(const std::string& path) {
  // file path correction checking is a concern of a client
  fs::path fPath{path};
  auto format = fPath.extension().string();
  auto serviceName = serviceDict_.findService(format);
  if (!serviceName.has_value()) {
    throwError(
        name_, "OpenFile",
        "There is no suitable service running for opening file " + path + ".");
  }
  openFileUsingServiceHandler(path, *serviceName);
}

void RegisteringService::openFileUsingServiceHandler(
    const std::string& path, const std::string& service) {
  auto serviceProxy =
      sdbus::createProxy(sdbus::ServiceName(service), sdbus::ObjectPath("/"));
  sdbus::InterfaceName interface { service };
  try {
    serviceProxy->callMethod("OpenFile")
        .onInterface(interface)
        .withArguments(path);
  } catch (const sdbus::Error& error) {
    std::cerr << error.what() << std::endl;
  }
}

}  // namespace shr