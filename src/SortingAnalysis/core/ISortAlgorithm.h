#pragma once

#include <string>
#include <vector>
#include "SortStatistics.h"

class ISortAlgorithm {
public:
    virtual ~ISortAlgorithm() = default;

    virtual std::string name() const = 0;
    virtual std::string fileName() const = 0;
    virtual std::string description() const = 0;

    virtual void sort(std::vector<int>& array, SortStatistics& stats) const = 0;
};
