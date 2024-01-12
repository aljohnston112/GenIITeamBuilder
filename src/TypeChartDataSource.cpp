#include <sstream>
#include <fstream>
#include <filesystem>
#include "TypeChartDataSource.h"
#include "data_class/PokemonType.h"
#include "rapidjson/stream.h"
#include "rapidjson/document.h"

using TypeChart = std::unordered_map<PokemonType, std::vector<PokemonType>>;

std::vector<TypeChart> TypeChartDataSource::defendTypeChart = std::vector<TypeChart>();
std::mutex TypeChartDataSource::mutex;

std::stringstream TypeChartDataSource::readDefenderTypesFile() {
    std::stringstream buffer;
    auto allPokemonFile = freshDefenderTypeFile;
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

std::vector<TypeChart>& TypeChartDataSource::getDefendTypeDict() {
    mutex.lock();
    if (defendTypeChart.empty()) {
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
                const auto key = convertToPokemonType(entry.name.GetString());
                std::vector<PokemonType> types;
                for (const auto &type: entry.value.GetArray()) {
                    types.emplace_back(convertToPokemonType(type.GetString()));
                }
                typeChart[key] = types;
            }
            defendTypeChart.push_back(typeChart);
        }
    }
    mutex.unlock();
    return defendTypeChart;
}
