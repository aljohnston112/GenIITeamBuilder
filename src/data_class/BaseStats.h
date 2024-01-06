#include <string>
#include <utility>
#include "Stats.h"

struct BaseStats {

    const Stats stats;

    explicit BaseStats(Stats stats) : stats(std::move(stats)) {}

};