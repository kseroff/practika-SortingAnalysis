#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class HeapSort final : public ISortAlgorithm {
private:
    void heapify(std::vector<int>& array, std::size_t heapSize, std::size_t root, SortStatistics& stats) const {
        std::size_t largest = root;
        std::size_t left = 2 * root + 1;
        std::size_t right = 2 * root + 2;

        if (left < heapSize) {
            ++stats.comparisons;
            if (array[left] > array[largest]) {
                largest = left;
            }
        }

        if (right < heapSize) {
            ++stats.comparisons;
            if (array[right] > array[largest]) {
                largest = right;
            }
        }

        if (largest != root) {
            std::swap(array[root], array[largest]);
            ++stats.swaps;
            heapify(array, heapSize, largest, stats);
        }
    }

public:
    std::string name() const override { return "Heap Sort"; }
    std::string fileName() const override { return "heap-sort"; }
    std::string description() const override { return "Пирамидальная сортировка через двоичную кучу."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        const std::size_t n = array.size();
        if (n < 2) {
            return;
        }

        for (int i = static_cast<int>(n / 2) - 1; i >= 0; --i) {
            heapify(array, n, static_cast<std::size_t>(i), stats);
        }

        for (std::size_t i = n - 1; i > 0; --i) {
            std::swap(array[0], array[i]);
            ++stats.swaps;
            heapify(array, i, 0, stats);
        }
    }
};

REGISTER_SORT_ALGORITHM(HeapSort);
