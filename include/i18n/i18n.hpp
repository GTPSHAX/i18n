#ifndef I18N_HPP
#define I18N_HPP

#include "core.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

/**
 * @brief Internationalization (i18n) struct for managing translations.
 *
 * This struct utilizes the nlohmann::json library to handle translation data.
 * It supports multiple locales and provides methods for loading and accessing translations.
 *
 * Example usage:
 * @code{.cpp}
 * #include <nlohmann/json.hpp>
 * #include <i18n/i18n.hpp>
 *
 * // Create translations JSON
 * nlohmann::json json = {
 *   {"en", {{"greeting", "Hello"}}},
 *   {"id", {{"greeting", "Halo"}}}
 * };
 *
 * // Initialize with JSON object
 * I18n i18n(json);
 *
 * // Or initialize from file
 * I18n i18n_from_file("path/to/translations.json");
 *
 * // Get translation in English
 * std::string greeting_en = i18n.t("greeting", "en");
 * std::cout << greeting_en << std::endl; // Output: Hello
 * 
 * // Get translation in Indonesian
 * std::string greeting_id = i18n.t("greeting", "id");
 * std::cout << greeting_id << std::endl; // Output: Halo
 * @endcode
 */
struct I18n
{
private:
  /**
   * @brief JSON object containing all translation data organized by locale
   */
  nlohmann::json translations;
  
  /**
   * @brief List of available locale codes
   */
  std::vector<std::string> locales;

  /**
   * @brief Resolve a dot-separated path in a nlohmann::json object.
   *
   * This function navigates through a JSON object using a dot-separated path
   * and returns a pointer to the corresponding JSON value. If the path does not
   * exist, it returns nullptr.
   *
   * @param source The source JSON object to navigate.
   * @param path The dot-separated path to resolve (e.g., "user.name.first").
   * @return A pointer to the resolved JSON value, or nullptr if the path does not exist.
   */
  const nlohmann::json *resolvePath(const nlohmann::json &source, const std::string &path)
  {
    const nlohmann::json *current = &source;
    std::stringstream ss(path);
    std::string segment;

    while (std::getline(ss, segment, '.'))
    {
      if (!current->is_object() || !current->contains(segment))
      {
        return nullptr;
      }
      current = &((*current)[segment]);
    }

    return current;
  }

  /**
   * @brief Check if content is available in other locales.
   *
   * This function checks if a given path has non-null content in any locale
   * other than the specified current language.
   *
   * @param path The dot-separated path to check (e.g., "user.name.first").
   * @param currentLang The current language code to exclude from the check (e.g., "en").
   * @return true if content is available in other locales, false otherwise.
   */
  bool isContentAvailableInOtherLocales(const std::string &path, const std::string &currentLang)
  {
    for (const auto &locale : locales)
    {
      if (locale != currentLang)
      {
        const nlohmann::json *node = resolvePath(translations[locale], path);
        if (node && !node->is_null())
        {
          return true;
        }
      }
    }
    return false;
  }

public:
  /**
   * @brief Default construct a new I18n object
   */
  I18n() = default;

  /**
   * @brief Construct a new I18n object from a file path
   * 
   * Loads translation data from a JSON file. The file must contain a valid JSON object
   * with locale codes as keys and translation objects as values.
   * 
   * @param filePath Path to the JSON file containing translations
   * @throws std::runtime_error If the file cannot be opened, is empty, or contains invalid JSON
   */
  I18n(const std::string &filePath)
  {
    std::ifstream ifs(filePath);
    if (!ifs.is_open())
    {
      throw std::runtime_error("Could not open file: " + filePath);
    }

    // if file is empty, throw error
    if (ifs.peek() == std::ifstream::traits_type::eof())
    {
      throw std::runtime_error("File is empty: " + filePath);
    }

    // if file size is 0, throw error
    ifs.seekg(0, std::ios::end);
    if (ifs.tellg() == 0)
    {
      throw std::runtime_error("File is empty: " + filePath);
    }

    // if file is not valid json, throw error
    try
    {
      nlohmann::json j = nlohmann::json::parse(ifs);
      translations = j;
      for (auto &[key, value] : j.items())
      {
        locales.push_back(key);
      }
    }
    catch (const nlohmann::json::parse_error &e)
    {
      throw std::runtime_error("Invalid JSON in file: " + filePath + "\n" + e.what());
    }
  }

  /**
   * @brief Construct a new I18n object from a nlohmann::json object
   * 
   * Initializes the I18n system with pre-loaded translation data. The JSON object must
   * have locale codes as keys (e.g., "en", "id") and translation objects as values.
   * 
   * @param json A JSON object containing translation data organized by locale
   * @throws std::runtime_error If the JSON is not an object or is empty
   */
  I18n(const nlohmann::json &json)
  {
    // if json is not an object, throw error
    if (!json.is_object())
    {
      throw std::runtime_error("JSON must be an object");
    }

    // if json is empty, throw error
    if (json.empty())
    {
      throw std::runtime_error("JSON object is empty");
    }

    translations = json;
    for (auto &[key, value] : json.items())
    {
      locales.push_back(key);
    }
  }

  /**
   * @brief Destroy the I18n object
   */
  ~I18n() = default;

  /**
   * @brief Get a translation value with type conversion
   * 
   * Retrieves a translated value from the translation data for the specified path and language code.
   * If the requested translation is not found or is null, it falls back to the English ("en") translation.
   * If no valid translation is found, returns the provided default value.
   * 
   * @tparam T The type to convert the translation value to (e.g., std::string, int, bool)
   * @param path The dot-separated path to the translation key (e.g., "user.greeting")
   * @param langCode The language code to retrieve the translation for (e.g., "en", "id")
   * @param defaultValue The value to return if no translation is found
   * @return T The translated value cast to type T, or the default value if not found
   * 
   * @note Prints a warning to stderr if content is not available in any locale except the current one
   * @note Falls back to English ("en") if the requested language is not found
   */
  template <typename T>
  T get(const std::string &path, std::string langCode, T defaultValue)
  {
    const nlohmann::json *node = nullptr;
    const nlohmann::json *fallback = nullptr;

    if (translations.contains(langCode))
    {
      node = resolvePath(translations[langCode], path);
    }

    if (!isContentAvailableInOtherLocales(path, langCode))
    {
      std::cerr << "Warning: Content for path '" << path << "' is not available in any locale except '" << langCode << "'." << std::endl;
    }

    if (langCode != "en" && translations.contains("en"))
    {
      fallback = resolvePath(translations["en"], path);
    }

    const nlohmann::json *target = node && !node->is_null() ? node : fallback;
    if (!target || target->is_null())
    {
      return defaultValue;
    }

    try
    {
      return target->get<T>();
    }
    catch (const nlohmann::json::exception &)
    {
      return defaultValue;
    }
  }

  /**
   * @brief Translate a key to a value (shorthand method)
   * 
   * This is a convenience method that wraps the `get()` method with a more intuitive name
   * for translation operations. It retrieves a translated value for the specified path and language.
   * Automatically provides a default "Content not found" message for string types when no default is specified.
   * 
   * @param path The dot-separated path to the translation key (e.g., "messages.welcome")
   * @param langCode The language code (defaults to "en")
   * @param defaultValue The fallback value if translation is not found (defaults to T{}, or "Content not found" for strings)
   * @return T The translated value, or the default value if not found
   * 
   * @note For string types, if no default value is provided, returns "Content not found" instead of an empty string
   */
  template <typename T = std::string>
  T t(const std::string &path, std::string langCode = "en", T defaultValue = T{})
  {
    if constexpr (std::is_same_v<T, std::string>)
    {
      if (defaultValue.empty())
      {
        defaultValue = "Content not found";
      }
    }
    return get<T>(path, langCode, defaultValue);
  }
};

#endif // I18N_HPP