#ifndef TTO_SHARING_SERVICE_HPP
#define TTO_SHARING_SERVICE_HPP

#include "service.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <memory>
#include <string>

namespace shr {

class SharingService final : public IElementaryService {
public:
   template <typename Callable>
   SharingService(const std::string &name, const std::vector<std::string> &supportedFormats, Callable &&handler)
      : IElementaryService(name, "/"), supportedFormats_{supportedFormats}, handler_{std::move(handler)} {}

   SharingService(SharingService &&) = default;
   SharingService &operator=(SharingService &&) = default;

   SharingService(const SharingService &) = delete;
   SharingService &operator=(const SharingService &) = delete;

   void start() override;

private:
   std::vector<std::string> supportedFormats_;

   std::function<void(const std::string &)> handler_;
};

} // namespace shr

#endif // TTO_SHARING_SERVICE_HPP