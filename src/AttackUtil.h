#ifndef GENIITEAMBUILDER_ATTACKUTIL_H
#define GENIITEAMBUILDER_ATTACKUTIL_H

int getLowKickPower(double pounds) {
    int power = 0;

    if (pounds < 21.9) {
        power = 20;
    } else if (pounds < 55.1) {
        power = 40;
    } else if (pounds < 110.2) {
        power = 60;
    } else if (pounds < 220.4) {
        power = 80;
    } else if (pounds < 440.9) {
        power = 100;
    } else {
        power = 120;
    }

    return power;
}

#endif //GENIITEAMBUILDER_ATTACKUTIL_H
