#ifndef GENIITEAMBUILDER_TYPECHARTDATASOURCE_H
#define GENIITEAMBUILDER_TYPECHARTDATASOURCE_H

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include "data_class/PokemonType.h"

using TypeChart = std::unordered_map<PokemonType, std::vector<PokemonType>>;

class TypeChartDataSource {
private:
    static constexpr char freshDefenderTypeFile[] = "data/defender_types.json";
    static std::vector<TypeChart> defendTypeChart;
    static std::mutex mutex;

    static std::stringstream readDefenderTypesFile();

public:
    static std::vector<TypeChart>& getDefendTypeDict();
};

#endif //GENIITEAMBUILDER_TYPECHARTDATASOURCE_H
