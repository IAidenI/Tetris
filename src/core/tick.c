#include "core/tick.h"

static double last_fall = 0;

int tick_should_fall(int level) {
    double interval = pow((0.8 - (level - 1) * 0.007), (level - 1));
    double now = get_time();

    if (now - last_fall >= interval) {
        last_fall = now;
        return 1;
    }
    return 0;
}
