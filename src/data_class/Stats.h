#include <string>
#include <utility>

class Stats {

public:

    const int health;
    const int attack;
    const int defense;
    const int special_attack;
    const int special_defense;
    const int speed;

    Stats(
            int health,
            int attack,
            int defense,
            int special_attack,
            int special_defense,
            int speed
    ) : health(health),
        attack(attack),
        defense(defense),
        special_attack(special_attack),
        special_defense(special_defense),
        speed(speed) {}

};