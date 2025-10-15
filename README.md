# i18n

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![CMake](https://img.shields.io/badge/CMake-3.10+-blue.svg)](https://cmake.org/)

A lightweight, header-only internationalization (i18n) library for C++ that provides easy-to-use translation management with JSON-based locale support.

[View Documentation](https://gtpshax.github.io/i18n/)

## Features

- **Header-only**: No need for separate compilation
- **JSON-based**: Uses nlohmann/json for robust JSON parsing
- **Dot-notation paths**: Access nested translations with dot-separated keys like `main.content` or `main.title`
- **Type-safe**: Template-based translation retrieval with automatic type conversion
- **Error handling**: Comprehensive exception handling for invalid data
- **Well-documented**: Complete Doxygen documentation
- **Tested**: Includes unit tests and examples

## Table of Contents

- [Installation](#installation)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Usage Examples](#usage-examples)
- [Build Instructions](#build-instructions)
- [Testing](#testing)
- [Documentation](#documentation)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Option 1: Header-only (Recommended)

Simply copy the `include/i18n/` directory to your project and include the headers:

```cpp
#include <i18n/i18n.hpp>
```

### Option 2: CMake Integration

Add this project as a subdirectory or use FetchContent:

```cmake
# Using FetchContent
include(FetchContent)
FetchContent_Declare(
  i18n
  GIT_REPOSITORY https://github.com/GTPSHAX/i18n.git
  GIT_TAG main
)
FetchContent_MakeAvailable(i18n)

# Link to your target
target_link_libraries(your_target PRIVATE i18n)
```

## Quick Start

```cpp
#include <nlohmann/json.hpp>
#include <i18n/i18n.hpp>
#include <iostream>

int main() {
    // Create translation data
    nlohmann::json translations = {
        {"en", {
            {"greeting", "Hello"},
            {"farewell", "Goodbye"},
            {"user", {
                {"name", "Name"},
                {"email", "Email"}
            }}
        }},
        {"id", {
            {"greeting", "Halo"},
            {"farewell", "Selamat tinggal"},
            {"user", {
                {"name", "Nama"},
                {"email", "Email"}
            }}
        }}
    };

    // Initialize i18n
    I18n i18n(translations);

    // Use translations
    std::cout << i18n.t("greeting", "en") << std::endl;    // "Hello"
    std::cout << i18n.t("greeting", "id") << std::endl;    // "Halo"
    std::cout << i18n.t("user.name", "en") << std::endl;   // "Name"

    return 0;
}
```

## API Reference

### Class: `I18n`

#### Constructors

- `I18n()` - Default constructor
- `I18n(const nlohmann::json& json)` - Initialize with JSON object
- `I18n(const std::string& filePath)` - Load translations from JSON file

#### Methods

- `T get<T>(const std::string& path, std::string langCode, T defaultValue)` - Get translation with type conversion
- `T t<T>(const std::string& path, std::string langCode = "en", T defaultValue = T{})` - Shorthand translation method

### Parameters

- `path`: Dot-separated path to translation key (e.g., `"user.name"`)
- `langCode`: Language code (e.g., `"en"`, `"id"`, `"fr"`)
- `defaultValue`: Fallback value if translation not found
- `T`: Template parameter for return type (automatically deduced)

## Usage Examples

### Loading from File

```cpp
// translations.json
{
  "en": {
    "welcome": "Welcome to our app!",
    "login": {
      "button": "Sign In",
      "username": "Username",
      "password": "Password"
    }
  },
  "es": {
    "welcome": "¡Bienvenido a nuestra aplicación!",
    "login": {
      "button": "Iniciar Sesión",
      "username": "Usuario",
      "password": "Contraseña"
    }
  }
}

// C++ code
I18n i18n("translations.json");
std::cout << i18n.t("welcome", "es") << std::endl;
```

### Type-safe Translation Retrieval

```cpp
I18n i18n(translations);

// String translations (default)
std::string greeting = i18n.t("greeting", "en");

// Integer translations
int maxUsers = i18n.t<int>("limits.users", "en", 100);

// Boolean translations
bool featureEnabled = i18n.t<bool>("features.logging", "en", false);

// Custom default values
std::string fallback = i18n.t("missing.key", "en", "Default text");
```

### Error Handling

```cpp
try {
    I18n i18n("nonexistent.json");
} catch (const std::runtime_error& e) {
    std::cerr << "Failed to load translations: " << e.what() << std::endl;
}

try {
    I18n i18n(invalid_json); // Empty or malformed JSON
} catch (const std::runtime_error& e) {
    std::cerr << "Invalid translation data: " << e.what() << std::endl;
}
```

## Build Instructions

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- [nlohmann/json](https://github.com/nlohmann/json) library

### Building

```bash
# Clone the repository
git clone https://github.com/GTPSHAX/i18n.git
cd i18n

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --config Release
```

### Integration with Your Project

Add to your `CMakeLists.txt`:

```cmake
# Add include directories
include_directories(path/to/i18n/include)

# Or add as subdirectory
add_subdirectory(path/to/i18n)

# Link the library (if built as shared/static)
target_link_libraries(your_target i18n)
```

## Testing

The project includes a test suite to verify functionality:

```bash
# From build directory
cd test
cmake --build . --config Release
./i18nTest
```

Expected output:
```
Hello
Halo
```

## Documentation

Complete API documentation is automatically generated using Doxygen and hosted on GitHub Pages.

### Local Documentation Generation

```bash
# Install Doxygen
# On Ubuntu/Debian:
sudo apt-get install doxygen

# On macOS:
brew install doxygen

# Generate documentation
doxygen Doxyfile

# Open documentation
open docs/html/index.html
```

## Dependencies

- **[nlohmann/json](https://github.com/nlohmann/json)** - JSON parsing library
- **C++17** - Required C++ standard
- **CMake 3.10+** - Build system (optional, header-only usage)

The nlohmann/json library is included as a git submodule in the `external/json/` directory.

## Project Structure

```
i18n-cpp/
├── include/i18n/           # Header files
│   ├── i18n.hpp           # Main library header
│   └── core.hpp           # Core definitions and dependencies
├── test/                   # Test suite
│   ├── CMakeLists.txt
│   └── src/main.cpp
├── external/               # External dependencies
│   └── json/              # nlohmann/json library
├── docs/                   # Generated documentation
├── .github/workflows/      # CI/CD workflows
├── CMakeLists.txt         # Main build configuration
├── Doxyfile              # Doxygen configuration
├── LICENSE.MIT           # MIT License
└── README.md             # This file
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### Development Setup

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Make your changes
4. Add tests if applicable
5. Ensure documentation is updated
6. Run tests: `cmake --build . --target test`
7. Commit your changes: `git commit -am 'Add some feature'`
8. Push to the branch: `git push origin feature/your-feature`
9. Submit a pull request

### Code Style

- Use C++17 features where appropriate
- Follow the existing code style
- Add documentation for new public APIs
- Include unit tests for new functionality

## License

This project is licensed under the MIT License - see the [LICENSE.MIT](LICENSE.MIT) file for details.

## Acknowledgments

- [nlohmann/json](https://github.com/nlohmann/json) - JSON library for modern C++
- [Doxygen](https://www.doxygen.nl/) - Documentation generation tool
- [CMake](https://cmake.org/) - Build system

## Support

If you find this library useful, please consider giving it a ⭐ star on GitHub!

For questions or issues, please [open an issue](https://github.com/GTPSHAX/i18n/issues) on GitHub.