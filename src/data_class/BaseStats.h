#include "Stats.h"

struct BaseStats {

    const Stats stats;

    explicit BaseStats(const Stats& stats) : stats(stats) {}

};