#pragma once

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include "StringUtils.h"

namespace Input {
    inline bool parseIntStrict(const std::string& text, int& value) {
        std::string trimmed = StringUtils::trim(text);
        if (trimmed.empty()) {
            return false;
        }

        std::istringstream stream(trimmed);
        long long temp = 0;
        char extra = '\0';

        if (!(stream >> temp)) {
            return false;
        }

        if (stream >> extra) {
            return false;
        }

        if (temp < std::numeric_limits<int>::min() || temp > std::numeric_limits<int>::max()) {
            return false;
        }

        value = static_cast<int>(temp);
        return true;
    }

    inline int readInt(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            std::string line;
            std::getline(std::cin, line);

            int value = 0;
            if (parseIntStrict(line, value)) {
                return value;
            }

            std::cout << "Ошибка: нужно ввести целое число. Попробуйте ещё раз.\n";
        }
    }

    inline int readIntInRange(const std::string& prompt, int minValue, int maxValue) {
        while (true) {
            int value = readInt(prompt);
            if (value >= minValue && value <= maxValue) {
                return value;
            }

            std::cout << "Ошибка: число должно быть от " << minValue << " до " << maxValue << ".\n";
        }
    }

    inline std::vector<int> readArrayFromLine() {
        while (true) {
            std::cout << "Введите целые числа через пробел: ";
            std::string line;
            std::getline(std::cin, line);

            auto tokens = StringUtils::splitBySpaces(line);
            if (tokens.empty()) {
                std::cout << "Ошибка: массив не может быть пустым.\n";
                continue;
            }

            std::vector<int> values;
            bool ok = true;

            for (const std::string& token : tokens) {
                int value = 0;
                if (!parseIntStrict(token, value)) {
                    std::cout << "Ошибка: '" << token << "' не является корректным целым числом.\n";
                    ok = false;
                    break;
                }
                values.push_back(value);
            }

            if (ok) {
                return values;
            }
        }
    }

    inline std::string readNonEmptyLine(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            std::string line;
            std::getline(std::cin, line);
            line = StringUtils::trim(line);

            if (!line.empty()) {
                return line;
            }

            std::cout << "Ошибка: строка не должна быть пустой.\n";
        }
    }

    inline bool isSafeFileBaseName(const std::string& name) {
        if (name.empty()) {
            return false;
        }

        for (unsigned char ch : name) {
            bool allowed = std::isalnum(ch) || ch == '_' || ch == '-';
            if (!allowed) {
                return false;
            }
        }

        return true;
    }

    inline std::string readFileBaseName(const std::string& prompt) {
        while (true) {
            std::string name = readNonEmptyLine(prompt);

            if (StringUtils::endsWith(name, ".txt")) {
                std::cout << "Введите только имя без расширения .txt. Расширение будет добавлено автоматически.\n";
                continue;
            }

            if (!isSafeFileBaseName(name)) {
                std::cout << "Ошибка: используйте только латинские буквы, цифры, дефис и подчёркивание.\n";
                std::cout << "Пример: random_100 или test-array-1\n";
                continue;
            }

            return name;
        }
    }
}
