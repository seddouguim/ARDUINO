#ifndef SRC_MANAGER
#define SRC_MANAGER

#include <Arduino.h>
#include "Cycle.h"
#include "Network.h"
#include "State.h"
#include "Display.h"

class Manager
{
public:
    Manager(size_t ncycles, Cycle *cycles, int emulation);
    void loop();
    Actions getActions();

private:
    int EMULATION_MODE;
    size_t ncycles;
    size_t current_cycle;
    bool is_running;
    unsigned long last_loop;
    void init();
    bool initialized;
    Cycle *cycles;
    State state;
    Network network;
    Display display;
    String get_current_cycle();
};

#endif /* SRC_MANAGER */
