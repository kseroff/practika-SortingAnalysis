#pragma once

#include "../core/SortRegistry.h"
#include <utility>

class CocktailSort final : public ISortAlgorithm {
public:
    std::string name() const override { return "Cocktail Sort"; }
    std::string fileName() const override { return "cocktail-sort"; }
    std::string description() const override { return "Шейкерная сортировка: проходы выполняются в двух направлениях."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        if (array.size() < 2) {
            return;
        }

        std::size_t left = 0;
        std::size_t right = array.size() - 1;
        bool swapped = true;

        while (swapped) {
            swapped = false;

            for (std::size_t i = left; i < right; ++i) {
                ++stats.comparisons;
                if (array[i] > array[i + 1]) {
                    std::swap(array[i], array[i + 1]);
                    ++stats.swaps;
                    swapped = true;
                }
            }

            if (!swapped) {
                break;
            }

            swapped = false;
            --right;

            for (std::size_t i = right; i > left; --i) {
                ++stats.comparisons;
                if (array[i - 1] > array[i]) {
                    std::swap(array[i - 1], array[i]);
                    ++stats.swaps;
                    swapped = true;
                }
            }

            ++left;
        }
    }
};

REGISTER_SORT_ALGORITHM(CocktailSort);
