#ifndef I18N_CORE_HPP
#define I18N_CORE_HPP

#include <nlohmann/json.hpp>

// Validate if nlohmann/json.hpp is available
#ifndef __has_include
  #error "__has_include not supported"
#elif !__has_include(<nlohmann/json.hpp>)
  #error "nlohmann/json.hpp not found, please install the nlohmann/json library"
#endif

#endif // I18N_CORE_HPP