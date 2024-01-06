#ifndef GENIITEAMBUILDER_CATEGORY_H
#define GENIITEAMBUILDER_CATEGORY_H

#include <string>

enum class Category {
    PHYSICAL,
    SPECIAL,
    STATUS
};

Category convertToAttackCategory(const std::string& pokemon_type);

#endif //GENIITEAMBUILDER_CATEGORY_H
