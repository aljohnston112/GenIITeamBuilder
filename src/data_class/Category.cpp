#include <unordered_set>
#include "Category.h"

Category convertToAttackCategory(const std::string& pokemon_type) {
    static const std::unordered_set<std::string> physical_types = {
            "normal", "fighting", "poison", "ground", "flying", "bug", "rock", "ghost", "steel"
    };
    static const std::unordered_set<std::string> special_types = {
            "fire", "water", "electric", "grass", "ice", "psychic", "dragon", "dark"
    };

    if (physical_types.find(pokemon_type) != physical_types.end()) {
        return Category::PHYSICAL;
    } else if (special_types.find(pokemon_type) != special_types.end()) {
        return Category::SPECIAL;
    } else {
        return Category::STATUS;
    }

}