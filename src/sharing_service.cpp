#include "sharing_service.hpp"

#include <iostream>

namespace shr {

void SharingService::start() {
  const auto [connection, object] =
      detail::setupSessionConnection(name_, objectPath_);
  object
      ->addVTable(sdbus::registerMethod("OpenFile")
                      .withInputParamNames("path")
                      .implementedAs(std::move(handler_)))
      .forInterface(name_);

  const auto regServiceName = "com.system.sharing";
  const auto regServiceProxy = sdbus::createProxy(
      sdbus::ServiceName{regServiceName}, sdbus::ObjectPath{"/"});
  const sdbus::InterfaceName regServiceIface{regServiceName};
  try {
    regServiceProxy->callMethod("RegisterService")
        .onInterface(regServiceIface)
        .withArguments(name_, supportedFormats_);
  } catch (const sdbus::Error& error) {
    std::cerr << error.what() << std::endl;
  }
  connection->enterEventLoop();
}

}  // namespace shr