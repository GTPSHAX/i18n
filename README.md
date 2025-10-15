# i18n

A lightweight internationalization (i18n) library written in C++ that simplifies managing translations for multiple locales using JSON.

## Features

- **Simple API**: Easy-to-use interface for loading and retrieving translations.
- **JSON Support**: Utilizes the nlohmann/json library for handling translation data.
- **Multiple Locales**: Supports multiple language codes (e.g., "en", "id").
- **Fallback Mechanism**: Automatically falls back to English ("en") if the requested translation is not available.
- **File and In-Memory Loading**: Load translations from JSON files or directly from nlohmann::json objects.
- **Type-Safe Retrieval**: Template-based methods for retrieving translations with default values.
- **Warning System**: Warns if content is only available in one locale.

## Installation

### Prerequisites

- C++17 or later
- [nlohmann/json](https://github.com/nlohmann/json) library (included as a submodule in `external/json`)

### Building

1. Clone the repository with submodules:
   ```bash
   git clone --recursive https://github.com/GTPSHAX/i18n-cpp.git
   cd i18n-cpp
   ```

2. Build using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Include the headers in your project:
   ```cpp
   #include <i18n/i18n.hpp>
   ```

## Usage

### Basic Example

```cpp
#include <i18n/i18n.hpp>
#include <iostream>

int main() {
    // Create translation data
    nlohmann::json translations = {
        {"en", {{"greeting", "Hello"}, {"farewell", "Goodbye"}}},
        {"id", {{"greeting", "Halo"}, {"farewell", "Selamat tinggal"}}}
    };

    // Initialize I18n object
    I18n i18n(translations);

    // Retrieve translations
    std::cout << i18n.t("greeting", "en") << std::endl; // Output: Hello
    std::cout << i18n.t("greeting", "id") << std::endl; // Output: Halo

    // With default value
    std::cout << i18n.t("unknown", "en", "Default") << std::endl; // Output: Default

    return 0;
}
```

### Loading from File

```cpp
#include <i18n/i18n.hpp>

int main() {
    try {
        // Load from JSON file
        I18n i18n("path/to/translations.json");

        // Use translations
        std::string greeting = i18n.t("greeting", "en");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

### JSON File Format

Your `translations.json` should be structured like this:

```json
{
  "en": {
    "greeting": "Hello",
    "farewell": "Goodbye",
    "user": {
      "name": "Name",
      "age": "Age"
    }
  },
  "id": {
    "greeting": "Halo",
    "farewell": "Selamat tinggal",
    "user": {
      "name": "Nama",
      "age": "Usia"
    }
  }
}
```

### Advanced Usage

```cpp
// Retrieve nested translations
std::string userName = i18n.t("user.name", "en");

// Get with specific type
int age = i18n.get("user.age", "en", 0); // Returns int, default 0

// Check availability in other locales
// (Warnings are printed automatically if content is only in one locale)
```

## API Reference

For detailed API documentation, see the [Doxygen-generated docs](docs/html/index.html).

### Key Classes

- `I18n`: Main class for managing translations.

### Key Methods

- `I18n(const std::string& filePath)`: Constructor loading from JSON file.
- `I18n(const nlohmann::json& json)`: Constructor from JSON object.
- `template<typename T> T get(const std::string& path, std::string langCode, T defaultValue)`: Retrieve translation with type and default.
- `template<typename T> T t(const std::string& path, std::string langCode, T defaultValue)`: Shorthand for get, with default language handling.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE.MIT](LICENSE.MIT) file for details.

## Dependencies

- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++
