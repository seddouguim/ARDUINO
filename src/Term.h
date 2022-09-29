#ifndef SRC_TERM
#define SRC_TERM

#include <Arduino.h>
#include "utils.h"
#include "config.h"

class Term
{
public:
    Term(String name, Duration duration, Actions actions);
    bool run(int emulation);
    unsigned long getDuration();
    Actions getActions();

private:
    String name;
    unsigned long duration;
    unsigned long t_duration;
    unsigned long end_time;
    bool isRunning;
    Actions actions;
};

#endif /* SRC_TERM */
