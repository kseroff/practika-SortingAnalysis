#pragma once

#include "../core/SortRegistry.h"

class ShellSort final : public ISortAlgorithm {
public:
    std::string name() const override { return "Shell Sort"; }
    std::string fileName() const override { return "shell-sort"; }
    std::string description() const override { return "Сортировка Шелла с постепенным уменьшением шага."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        const std::size_t n = array.size();

        for (std::size_t gap = n / 2; gap > 0; gap /= 2) {
            for (std::size_t i = gap; i < n; ++i) {
                int temp = array[i];
                ++stats.moves;

                std::size_t j = i;
                while (j >= gap) {
                    ++stats.comparisons;
                    if (array[j - gap] <= temp) {
                        break;
                    }

                    array[j] = array[j - gap];
                    ++stats.moves;
                    j -= gap;
                }

                array[j] = temp;
                ++stats.moves;
            }
        }
    }
};

REGISTER_SORT_ALGORITHM(ShellSort);
