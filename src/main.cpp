#include "MyHandler.h"
#include "Timer.h"
#include "BattleField.h"



int main() {
    BattleField battleField;
    // Things that go into the lambda must have scope that lasts as long as the method battleTime
    logFunctionTime(
            [&battleField] {
                battleField.battleTime();
                return "";
            },
            "Program run time: "
    );

    return 0;
}
