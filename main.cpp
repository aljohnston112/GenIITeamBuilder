#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

#include "rapidjson/document.h"
#include "MyHandler.h"

std::stringstream read_all_pokemon();

using namespace rapidjson;

namespace FileNames {
    const std::string ALL_POKEMON_FILE = "data/all_pokemon.json";
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string allPokemonJSON = read_all_pokemon().str();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "File read time: " << duration.count() << std::endl;
    start = std::chrono::high_resolution_clock::now();

    StringStream buffer(allPokemonJSON.c_str());
    stop = std::chrono::high_resolution_clock::now();
    duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "StringStream fill time: " << duration.count() << std::endl;
    start = std::chrono::high_resolution_clock::now();

    MyHandler handler;
    Reader reader;
    reader.Parse(buffer, handler);
    stop = std::chrono::high_resolution_clock::now();
    duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Parse time: " << duration.count() << std::endl;

    return 0;
}

std::stringstream read_all_pokemon() {
    std::stringstream buffer;
    std::string allPokemonFile = FileNames::ALL_POKEMON_FILE;
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
