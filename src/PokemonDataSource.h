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
    static constexpr char ALL_POKEMON_FILE[] = "data/all_pokemon.json";
    static Document document;

    static std::stringstream readAllPokemonFile() {
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

    static void initializeDocument(){
        std::string allPokemonJSON = readAllPokemonFile().str();
        StringStream buffer(allPokemonJSON.c_str());
        document.ParseStream(buffer);
    }

    static std::vector<std::string> getAllPokemonNames() {
        std::vector<std::string> pokemonNames{};
        pokemonNames.reserve(251);
        for (auto &m: document.GetObject()) {
            pokemonNames.emplace_back(m.name.GetString());
        }
        return pokemonNames;
    }

    static auto& getPokemonObjectByIndex(const std::string& id){
        auto pokemon = document.FindMember(id.c_str());
        return pokemon->value;
    }

};

#endif //GENIITEAMBUILDER_POKEMONDATASOURCE_H
