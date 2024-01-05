#ifndef GENIITEAMBUILDER_POKEMONDATASOURCE_H
#define GENIITEAMBUILDER_POKEMONDATASOURCE_H

#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "rapidjson/document.h"

using namespace rapidjson;

class PokemonDataSource {

private:
    static const std::string ALL_POKEMON_FILE;

    static std::stringstream read_all_pokemon() {
        std::stringstream buffer;
        auto allPokemonFile = ALL_POKEMON_FILE;
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

public:
    static std::vector<std::string> getAllPokemonNames() {
        std::string allPokemonJSON = read_all_pokemon().str();
        StringStream buffer(allPokemonJSON.c_str());
        Document document;
        document.ParseStream(buffer);
        std::vector<std::string> pokemonNames{};
        pokemonNames.reserve(251);
        for (auto &m: document.GetObject()) {
            pokemonNames.emplace_back(m.value["pokemon_information"]["name"].GetString());
        }
        return pokemonNames;
    }

};

#endif //GENIITEAMBUILDER_POKEMONDATASOURCE_H
