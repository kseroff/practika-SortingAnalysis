#pragma once

#include "../core/SortRegistry.h"
#include <algorithm>
#include <stdexcept>

class RadixSort final : public ISortAlgorithm {
private:
    void countSortByDigit(std::vector<int>& array, int exp, SortStatistics& stats) const {
        std::vector<int> output(array.size());
        int count[10] = { 0 };

        for (int value : array) {
            int digit = (value / exp) % 10;
            ++count[digit];
            ++stats.moves;
        }

        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = static_cast<int>(array.size()) - 1; i >= 0; --i) {
            int digit = (array[static_cast<std::size_t>(i)] / exp) % 10;
            output[static_cast<std::size_t>(count[digit] - 1)] = array[static_cast<std::size_t>(i)];
            --count[digit];
            ++stats.moves;
        }

        for (std::size_t i = 0; i < array.size(); ++i) {
            array[i] = output[i];
            ++stats.moves;
        }
    }

public:
    std::string name() const override { return "Radix Sort"; }
    std::string fileName() const override { return "radix-sort"; }
    std::string description() const override { return "Поразрядная сортировка для неотрицательных целых чисел."; }

    void sort(std::vector<int>& array, SortStatistics& stats) const override {
        if (array.empty()) {
            return;
        }

        for (int value : array) {
            if (value < 0) {
                throw std::runtime_error("Radix Sort поддерживает только неотрицательные числа.");
            }
        }

        int maxValue = *std::max_element(array.begin(), array.end());
        for (int exp = 1; maxValue / exp > 0; exp *= 10) {
            countSortByDigit(array, exp, stats);
            if (exp > 100000000) {
                break;
            }
        }
    }
};

REGISTER_SORT_ALGORITHM(RadixSort);
