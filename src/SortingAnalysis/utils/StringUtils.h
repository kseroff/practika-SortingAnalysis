#pragma once

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

namespace StringUtils {
    inline std::string trim(const std::string& text) {
        std::size_t start = 0;
        while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) {
            ++start;
        }

        std::size_t end = text.size();
        while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
            --end;
        }

        return text.substr(start, end - start);
    }

    inline std::vector<std::string> splitBySpaces(const std::string& line) {
        std::vector<std::string> tokens;
        std::istringstream stream(line);
        std::string token;

        while (stream >> token) {
            tokens.push_back(token);
        }

        return tokens;
    }

    inline bool endsWith(const std::string& text, const std::string& suffix) {
        if (suffix.size() > text.size()) {
            return false;
        }

        return std::equal(suffix.rbegin(), suffix.rend(), text.rbegin());
    }
}
