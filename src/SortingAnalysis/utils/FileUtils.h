#pragma once

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Input.h"
#include "StringUtils.h"

namespace FileUtils {
    inline std::string makeTxtFileName(const std::string& baseName) {
        return baseName + ".txt";
    }

    inline std::string makeSortedFileName(const std::string& baseName, const std::string& sortFileName) {
        return baseName + "-sorted-" + sortFileName + ".txt";
    }

    inline void saveArrayToFile(const std::vector<int>& array, const std::string& fileName) {
        std::ofstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + fileName);
        }

        for (std::size_t i = 0; i < array.size(); ++i) {
            file << array[i];
            if (i + 1 < array.size()) {
                file << ' ';
            }
        }

        if (!file.good()) {
            throw std::runtime_error("Ошибка записи в файл: " + fileName);
        }
    }

    inline std::vector<int> loadArrayFromFile(const std::string& fileName) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для чтения: " + fileName);
        }

        std::vector<int> result;
        std::string token;

        while (file >> token) {
            int value = 0;
            if (!Input::parseIntStrict(token, value)) {
                throw std::runtime_error("Файл содержит некорректное значение: " + token);
            }
            result.push_back(value);
        }

        if (file.bad()) {
            throw std::runtime_error("Ошибка чтения файла: " + fileName);
        }

        if (result.empty()) {
            throw std::runtime_error("Файл пустой или не содержит чисел: " + fileName);
        }

        return result;
    }
}
