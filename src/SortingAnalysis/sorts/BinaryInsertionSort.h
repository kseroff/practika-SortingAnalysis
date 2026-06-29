#pragma once

#include "../core/SortRegistry.h"

class BinaryInsertionSort final : public ISortAlgorithm {
private:
    std::size_t binarySearchPosition(const std::vector<int>& array, int value, std::size_t right, SortStatistics& stats) const {
        std::size_t left = 0;

        while (left < right) {
            std::size_t middle = left + (right - left) / 2;
            ++stats.comparisons;

            if (value < array[middle]) {
                right = middle;
            } else {
                left = middle + 1;
            }
        }

        return left;
    }

public:
    std::string name() const override { return "Binary Insertion Sort"; }
    std::string fileName() const override { return "binary-insertion-sort"; }
    std::string description() const override { return "Сортировка вставками с бинарным поиском позиции вставки."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        for (std::size_t i = 1; i < array.size(); ++i) {
            int key = array[i];
            ++stats.moves;

            std::size_t position = binarySearchPosition(array, key, i, stats);

            for (std::size_t j = i; j > position; --j) {
                array[j] = array[j - 1];
                ++stats.moves;
            }

            array[position] = key;
            ++stats.moves;
        }
    }
};

REGISTER_SORT_ALGORITHM(BinaryInsertionSort);
