#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class BubbleSortSwapFlag final : public ISortAlgorithm {
public:
    std::string name() const override { return "Bubble Sort with swap flag"; }
    std::string fileName() const override { return "bubble-sort-swap-flag"; }
    std::string description() const override { return "Пузырьковая сортировка с досрочной остановкой, если обменов не было."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        const std::size_t n = array.size();
        for (std::size_t i = 0; i < n; ++i) {
            bool swapped = false;
            for (std::size_t j = 0; j + 1 < n - i; ++j) {
                ++stats.comparisons;
                if (array[j] > array[j + 1]) {
                    std::swap(array[j], array[j + 1]);
                    ++stats.swaps;
                    swapped = true;
                }
            }

            if (!swapped) {
                break;
            }
        }
    }
};

REGISTER_SORT_ALGORITHM(BubbleSortSwapFlag);
