#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class SelectionSort final : public ISortAlgorithm {
public:
    std::string name() const override { return "Selection Sort"; }
    std::string fileName() const override { return "selection-sort"; }
    std::string description() const override { return "Сортировка выбором минимального элемента."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        const std::size_t n = array.size();
        for (std::size_t i = 0; i < n; ++i) {
            std::size_t minIndex = i;
            for (std::size_t j = i + 1; j < n; ++j) {
                ++stats.comparisons;
                if (array[j] < array[minIndex]) {
                    minIndex = j;
                }
            }

            if (minIndex != i) {
                std::swap(array[i], array[minIndex]);
                ++stats.swaps;
            }
        }
    }
};

REGISTER_SORT_ALGORITHM(SelectionSort);
