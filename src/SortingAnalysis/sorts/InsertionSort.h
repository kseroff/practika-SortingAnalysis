#pragma once

#include "../core/SortRegistry.h"

class InsertionSort final : public ISortAlgorithm {
public:
    std::string name() const override { return "Insertion Sort"; }
    std::string fileName() const override { return "insertion-sort"; }
    std::string description() const override { return "Сортировка вставками."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        for (std::size_t i = 1; i < array.size(); ++i) {
            int key = array[i];
            ++stats.moves;

            std::size_t j = i;
            while (j > 0) {
                ++stats.comparisons;
                if (array[j - 1] <= key) {
                    break;
                }

                array[j] = array[j - 1];
                ++stats.moves;
                --j;
            }

            array[j] = key;
            ++stats.moves;
        }
    }
};

REGISTER_SORT_ALGORITHM(InsertionSort);
