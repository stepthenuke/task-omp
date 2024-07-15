#ifndef TTO_REGISTERING_SERVICE_HPP
#define TTO_REGISTERING_SERVICE_HPP

#include <sdbus-c++/sdbus-c++.h>

#include <deque>
#include <memory>
#include <optional>
#include <string>

#include "service.hpp"

namespace shr {

namespace detail {

class FormatServiceDict {
 public:
  std::optional<std::string> findService(const std::string& format);
  void addService(const std::string& serviceName,
                  const std::vector<std::string>& supportedFormats);
  bool removeService(const std::string& serviceName);

 private:
  void removeServiceFromFormatMap(const std::string& format,
                                  const std::string& serviceName);

 private:
  std::unordered_map<std::string, std::vector<std::string>> serviceFormatMap_;
  // deque is used as we need to delete inside elements
  std::unordered_map<std::string, std::deque<std::string>> formatServiceMap_;
};

}  // namespace detail

class RegisteringService final : public IElementaryService {
 public:
  RegisteringService(const std::string& name = "com.system.sharing")
      : IElementaryService(name, "/") {}

  RegisteringService(RegisteringService&&) = default;
  RegisteringService& operator=(RegisteringService&&) = default;

  RegisteringService(const RegisteringService&) = delete;
  RegisteringService& operator=(const RegisteringService&) = delete;

  void start() override;

 private:
  detail::FormatServiceDict serviceDict_;

  void registerServiceHandler(const std::string& name,
                              const std::vector<std::string>& supportedFormats);
  void openFileHandler(const std::string& path);
  void openFileUsingServiceHandler(const std::string& path,
                                   const std::string& service);
};

}  // namespace shr

#endif  // TTO_REGISTERING_SERVICE_HPP