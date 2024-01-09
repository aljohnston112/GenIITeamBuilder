#ifndef GENIITEAMBUILDER_TYPECHARTDATASOURCE_H
#define GENIITEAMBUILDER_TYPECHARTDATASOURCE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <filesystem>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "data_class/PokemonType.h"

using TypeChart = std::unordered_map<PokemonType, std::vector<PokemonType>>;

namespace type_chart_data_source {
    static constexpr char FRESH_DEFENDER_TYPE_FILE[] = "data/defender_types.json";
}

static std::stringstream readDefenderTypesFile() {
    std::stringstream buffer;
    auto allPokemonFile = type_chart_data_source::FRESH_DEFENDER_TYPE_FILE;
    std::ifstream file(allPokemonFile);
    if (!file.is_open()) {
        throw std::filesystem::filesystem_error(
                "Unable to open file.",
                std::filesystem::path(allPokemonFile),
                std::make_error_code(std::errc::no_such_file_or_directory));
    }
    buffer << file.rdbuf();
    return buffer;
}

class TypeChartDataSource {
private:
    std::shared_ptr<std::vector<TypeChart>> defendTypeChart = make_shared<std::vector<TypeChart>>();
    std::mutex mutex;

public:
    std::shared_ptr<std::vector<TypeChart>> getDefendTypeDict() {
        mutex.lock();
        if (defendTypeChart->empty()) {
            printf("Pokemon type file loaded\n");
            rapidjson::Document document;
            std::string allPokemonJSON = readDefenderTypesFile().str();
            rapidjson::StringStream buffer(allPokemonJSON.c_str());
            document.ParseStream(buffer);

            auto defaultMultiplier = [] { return std::vector<PokemonType>(); };
            for (const auto &typeDict: document.GetArray()) {
                TypeChart typeChart;
                for (int i = 0; i < static_cast<int>(PokemonType::Count); ++i) {
                    auto enumValue = static_cast<PokemonType>(i);
                    typeChart[enumValue] = defaultMultiplier();
                }
                for (const auto &entry: typeDict.GetObject()) {
                    const auto key = convert_to_pokemon_type(entry.name.GetString());
                    std::vector<PokemonType> types;
                    for (const auto &type: entry.value.GetArray()) {
                        types.emplace_back(convert_to_pokemon_type(type.GetString()));
                    }
                    typeChart[key] = types;
                }
                defendTypeChart->push_back(typeChart);
            }
        }
        mutex.unlock();
        return defendTypeChart;
    }

};

#endif //GENIITEAMBUILDER_TYPECHARTDATASOURCE_H
