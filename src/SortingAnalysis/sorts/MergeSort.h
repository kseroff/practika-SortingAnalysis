#pragma once

#include "../core/SortRegistry.h"

class MergeSort final : public ISortAlgorithm {
private:
    void merge(std::vector<int>& array, std::vector<int>& buffer, std::size_t left, std::size_t mid, std::size_t right, SortStatistics& stats) const {
        std::size_t i = left;
        std::size_t j = mid;
        std::size_t k = left;

        while (i < mid && j < right) {
            ++stats.comparisons;
            if (array[i] <= array[j]) {
                buffer[k++] = array[i++];
            } else {
                buffer[k++] = array[j++];
            }
            ++stats.moves;
        }

        while (i < mid) {
            buffer[k++] = array[i++];
            ++stats.moves;
        }

        while (j < right) {
            buffer[k++] = array[j++];
            ++stats.moves;
        }

        for (std::size_t index = left; index < right; ++index) {
            array[index] = buffer[index];
            ++stats.moves;
        }
    }

    void mergeSort(std::vector<int>& array, std::vector<int>& buffer, std::size_t left, std::size_t right, SortStatistics& stats) const {
        if (right - left < 2) {
            return;
        }

        std::size_t mid = left + (right - left) / 2;
        mergeSort(array, buffer, left, mid, stats);
        mergeSort(array, buffer, mid, right, stats);
        merge(array, buffer, left, mid, right, stats);
    }

public:
    std::string name() const override { return "Merge Sort"; }
    std::string fileName() const override { return "merge-sort"; }
    std::string description() const override { return "Сортировка слиянием."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        std::vector<int> buffer(array.size());
        mergeSort(array, buffer, 0, array.size(), stats);
    }
};

REGISTER_SORT_ALGORITHM(MergeSort);
