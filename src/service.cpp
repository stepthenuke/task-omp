#include "service.hpp"

namespace shr {

namespace detail {

std::pair<std::unique_ptr<sdbus::IConnection>, std::unique_ptr<sdbus::IObject>>
setupSessionConnection(const std::string &serviceName, const std::string &objectPath) {
   auto connection = sdbus::createSessionBusConnection(sdbus::ConnectionName(serviceName));
   auto object = sdbus::createObject(*connection, sdbus::ObjectPath(objectPath));
   return { std::move(connection), std::move(object) };
}

} // namespace detail

} // namespace shr