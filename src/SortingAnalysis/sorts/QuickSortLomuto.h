#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class QuickSortLomuto final : public ISortAlgorithm {
private:
    int partition(std::vector<int>& array, int low, int high, SortStatistics& stats) const {
        int pivot = array[high];
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            ++stats.comparisons;
            if (array[j] <= pivot) {
                ++i;
                if (i != j) {
                    std::swap(array[i], array[j]);
                    ++stats.swaps;
                }
            }
        }

        if (i + 1 != high) {
            std::swap(array[i + 1], array[high]);
            ++stats.swaps;
        }

        return i + 1;
    }

    void quickSort(std::vector<int>& array, int low, int high, SortStatistics& stats) const {
        if (low < high) {
            int pivotIndex = partition(array, low, high, stats);
            quickSort(array, low, pivotIndex - 1, stats);
            quickSort(array, pivotIndex + 1, high, stats);
        }
    }

public:
    std::string name() const override { return "Quick Sort Lomuto"; }
    std::string fileName() const override { return "quick-sort-lomuto"; }
    std::string description() const override { return "Быстрая сортировка со схемой разбиения Lomuto."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        if (!array.empty()) {
            quickSort(array, 0, static_cast<int>(array.size()) - 1, stats);
        }
    }
};

REGISTER_SORT_ALGORITHM(QuickSortLomuto);
