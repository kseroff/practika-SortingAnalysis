#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class QuickSortHoare final : public ISortAlgorithm {
private:
    int partition(std::vector<int>& array, int low, int high, SortStatistics& stats) const {
        int pivot = array[low + (high - low) / 2];
        int i = low - 1;
        int j = high + 1;

        while (true) {
            do {
                ++i;
                ++stats.comparisons;
            } while (array[i] < pivot);

            do {
                --j;
                ++stats.comparisons;
            } while (array[j] > pivot);

            if (i >= j) {
                return j;
            }

            std::swap(array[i], array[j]);
            ++stats.swaps;
        }
    }

    void quickSort(std::vector<int>& array, int low, int high, SortStatistics& stats) const {
        if (low < high) {
            int p = partition(array, low, high, stats);
            quickSort(array, low, p, stats);
            quickSort(array, p + 1, high, stats);
        }
    }

public:
    std::string name() const override { return "Quick Sort Hoare"; }
    std::string fileName() const override { return "quick-sort-hoare"; }
    std::string description() const override { return "Быстрая сортировка со схемой разбиения Hoare."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        if (!array.empty()) {
            quickSort(array, 0, static_cast<int>(array.size()) - 1, stats);
        }
    }
};

REGISTER_SORT_ALGORITHM(QuickSortHoare);
