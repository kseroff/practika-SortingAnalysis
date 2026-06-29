#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include "ISortAlgorithm.h"

class SortRegistry {
private:
    std::vector<std::unique_ptr<ISortAlgorithm>> algorithms;

    SortRegistry() = default;

public:
    SortRegistry(const SortRegistry&) = delete;
    SortRegistry& operator=(const SortRegistry&) = delete;

    static SortRegistry& instance() {
        static SortRegistry registry;
        return registry;
    }

    void add(std::unique_ptr<ISortAlgorithm> algorithm) {
        algorithms.push_back(std::move(algorithm));
    }

    const std::vector<std::unique_ptr<ISortAlgorithm>>& all() const {
        return algorithms;
    }

    const ISortAlgorithm& getByIndex(std::size_t index) const {
        if (index >= algorithms.size()) {
            throw std::out_of_range("Некорректный индекс сортировки");
        }
        return *algorithms[index];
    }
};

#define REGISTER_SORT_ALGORITHM(CLASS_NAME) \
    inline const bool CLASS_NAME##_registered = []() { \
        SortRegistry::instance().add(std::make_unique<CLASS_NAME>()); \
        return true; \
    }()
