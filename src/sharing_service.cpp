#include "sharing_service.hpp"

namespace shr {

void SharingService::start() {
   auto [connection, object] = detail::setupSessionConnection(name_, objectPath_);
   object->addVTable(
      sdbus::registerMethod("OpenFile").implementedAs(std::move(handler_))
   ).forInterface(name_);

   const auto regServiceName = "com.system.sharing";
   auto regServiceProxy = sdbus::createProxy(sdbus::ServiceName{regServiceName}, sdbus::ObjectPath{"/"});
   sdbus::InterfaceName regServiceIface{regServiceName};
   {
      regServiceProxy->callMethod("RegisterService")
         .onInterface(regServiceIface)
         .withArguments(name_, supportedFormats_);
   }
   connection->enterEventLoop();
}

} // namespace shr