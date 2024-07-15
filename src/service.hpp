#ifndef TTO_SERVICE_HPP
#define TTO_SERVICE_HPP

#include <sdbus-c++/sdbus-c++.h>

#include <string>
#include <memory>

namespace shr {

namespace detail {

std::pair<std::unique_ptr<sdbus::IConnection>, std::unique_ptr<sdbus::IObject>>
setupSessionConnection(const std::string &serviceName, const std::string &objectPath);

} // namespace detail

void throwError(const std::string &serviceName, const std::string &methodName, const std::string &message);

// Minimal interface for a DBus service.
class IService {
public:
   virtual ~IService() = default;
   virtual void start() = 0;

protected:
   explicit IService(const std::string &name)
      : name_{name} 
   {}

protected:
   std::string name_;
};

// Interface for a service with single connection, object, and interface (name = interface).
// More concretely -- base for both SharingService and RegisteringService
class IElementaryService : public IService {
protected:
   IElementaryService(const std::string &name, const std::string &objectPath)
      : IService(name), objectPath_(objectPath) {}

protected:
   std::string objectPath_;
};

} // namespace shr

#endif // TTO_SERVICE_HPP