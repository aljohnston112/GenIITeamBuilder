#include "rapidjson/document.h"
#include "PokemonDataSource.h"

rapidjson::Document PokemonDataSource::document;
std::vector<Pokemon> PokemonDataSource::ALL_POKEMON;
