#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class BubbleSort final : public ISortAlgorithm {
public:
    std::string name() const override { return "Bubble Sort"; }
    std::string fileName() const override { return "bubble-sort"; }
    std::string description() const override { return "Обычная пузырьковая сортировка."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        const std::size_t n = array.size();
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j + 1 < n - i; ++j) {
                ++stats.comparisons;
                if (array[j] > array[j + 1]) {
                    std::swap(array[j], array[j + 1]);
                    ++stats.swaps;
                }
            }
        }
    }
};

REGISTER_SORT_ALGORITHM(BubbleSort);
