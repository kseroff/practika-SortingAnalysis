#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

namespace ArrayUtils {
    inline std::vector<int> generateRandomArray(int size, int minValue, int maxValue) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(minValue, maxValue);

        std::vector<int> array;
        array.reserve(static_cast<std::size_t>(size));

        for (int i = 0; i < size; ++i) {
            array.push_back(distribution(generator));
        }

        return array;
    }

    inline void printArray(const std::vector<int>& array, std::size_t maxToPrint = 100) {
        if (array.empty()) {
            std::cout << "Массив пуст.\n";
            return;
        }

        std::cout << "Размер массива: " << array.size() << '\n';
        std::cout << "Значения: ";

        std::size_t limit = (std::min)(array.size(), maxToPrint);
        for (std::size_t i = 0; i < limit; ++i) {
            std::cout << array[i];
            if (i + 1 < limit) {
                std::cout << ' ';
            }
        }

        if (array.size() > maxToPrint) {
            std::cout << " ...";
        }

        std::cout << "\n";
    }
}
