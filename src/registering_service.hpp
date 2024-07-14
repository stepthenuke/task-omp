#ifndef TTO_REGISTERING_SERVICE_HPP
#define TTO_REGISTERING_SERVICE_HPP

#include "service.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <memory>
#include <optional>
#include <string>

namespace shr {

namespace detail {

class FormatServiceDict {
public:
   std::optional<std::string> findService(const std::string &format);
   void pushService(const std::string &serviceName, const std::vector<std::string> &supportedFormats);

private:
   std::unordered_map<std::string, std::vector<std::string>> data_;
};

} // namespace detail

class RegisteringService final : public IElementaryService {
public:
   RegisteringService(const std::string &name = "com.system.sharing")
      : IElementaryService(name, "/") {}
   
   RegisteringService(RegisteringService &&) = default;
   RegisteringService &operator=(RegisteringService &&) = default;
   
   RegisteringService(const RegisteringService &) = delete;
   RegisteringService &operator=(const RegisteringService &) = delete;

   void start() override;

private:
   template <typename Callable>
   void registerMethod(const std::string &name, Callable &&handler);

private:
   detail::FormatServiceDict serviceDict_;

   void registerFileHandler(const std::string &name, const std::vector<std::string> &supportedFormats);
   void openFileHandler_(const std::string &path);
   void openFileUsingServiceHandler_(const std::string &);
};

} // namespace shr

#endif // TTO_REGISTERING_SERVICE_HPP