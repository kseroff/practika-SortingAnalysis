#pragma once

#include "../core/SortRegistry.h"
#include <algorithm>
#include <stdexcept>

class CountingSort final : public ISortAlgorithm {
public:
    std::string name() const override { return "Counting Sort"; }
    std::string fileName() const override { return "counting-sort"; }
    std::string description() const override { return "Сортировка подсчётом для целых чисел с ограниченным диапазоном."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        if (array.empty()) {
            return;
        }

        auto [minIt, maxIt] = std::minmax_element(array.begin(), array.end());
        int minValue = *minIt;
        int maxValue = *maxIt;

        long long range = static_cast<long long>(maxValue) - static_cast<long long>(minValue) + 1;
        if (range > 10000000) {
            throw std::runtime_error("Counting Sort отменён: диапазон значений слишком большой.");
        }

        std::vector<int> counts(static_cast<std::size_t>(range), 0);

        for (int value : array) {
            ++counts[static_cast<std::size_t>(static_cast<long long>(value) - minValue)];
            ++stats.moves;
        }

        std::size_t index = 0;
        for (std::size_t i = 0; i < counts.size(); ++i) {
            while (counts[i] > 0) {
                array[index++] = static_cast<int>(static_cast<long long>(i) + minValue);
                --counts[i];
                ++stats.moves;
            }
        }
    }
};

REGISTER_SORT_ALGORITHM(CountingSort);
