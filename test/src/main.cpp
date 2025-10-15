#include <i18n/i18n.hpp>
#include <iostream>

int main(){
  nlohmann::json json = {
    {"en", {{"greeting", "Hello"}}},
    {"id", {{"greeting", "Halo"}}}
  };

  try {
    // Initialize with local variable
    I18n i18n(json);

    std::cout << i18n.t("greeting", "en") << std::endl; // Output: Hello
    std::cout << i18n.t("greeting", "id") << std::endl; // Output: Halo
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}