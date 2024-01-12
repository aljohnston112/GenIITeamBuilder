#include "BattleSimulator.h"

int main() {
    BattleSimulator battleSimulator;
    // Things that go into the lambda must have scope that lasts as long as the method battleTime
    // battleTime opens several threads and runs lambdas on them for some time.
    Timer::logFunctionTime(
            [&battleSimulator] {
                battleSimulator.battleTime();
                return "";
            },
            "Program run time: "
    );

    return 0;
}
