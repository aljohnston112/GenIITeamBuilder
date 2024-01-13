#ifndef GENIITEAMBUILDER_COMBINATION_H
#define GENIITEAMBUILDER_COMBINATION_H

#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

class LexicographicalCombinationGenerator {
private:
    std::vector<size_t> indices;

public:
    const size_t setSize;
    const long combinationSize;

    LexicographicalCombinationGenerator(
            size_t set_size,
            long combination_size
    ) : setSize(set_size), combinationSize(combination_size) {
        for (size_t index = 0; index < combinationSize; ++index) {
            indices.push_back(index);
        }
    }

    bool increment() {
        if (indices[combinationSize - 1] < setSize - 1) {
            indices[combinationSize - 1]++;
            return true;
        }

        for (long i = combinationSize - 2; i >= 0; --i) {
            if (indices[i] < indices[i + 1] - 1) {
                indices[i]++;

                for (size_t j = i + 1; j < combinationSize; ++j) {
                    indices[j] = indices[j - 1] + 1;
                }

                return true;
            }
        }

        return false;
    }

    std::vector<size_t> getIndices() { return indices; }
};

#endif //GENIITEAMBUILDER_COMBINATION_H
