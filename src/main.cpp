#include "MyHandler.h"
#include "Timer.h"
#include "BattleField.h"

int main() {
    BattleField battleField;
    logFunctionTime(
            [&battleField] {
                battleField.battleTime();
                return "";
            },
            "Program run time: "
    );

    return 0;
}
