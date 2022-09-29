#ifndef SRC_CYCLE
#define SRC_CYCLE

#include <Arduino.h>
#include "utils.h"
#include "Term.h"
#include "config.h"
#include "ArduinoJson.h"

class Cycle
{
public:
    Cycle(String name, size_t nterms, Term *terms, Condition start_condition, Signal end_signal);
    Cycle(String name, size_t nterms, Term *terms, Signal end_signal);
    Cycle(String name, size_t nterms, Term *terms);
    int run(int emulation, StaticJsonDocument<256> current_state);
    bool is_running();
    Actions getActions();
    size_t get_current_term();

private:
    String name;
    unsigned long duration;
    unsigned long t_duration;
    unsigned long end_time;
    int isRunning;
    size_t nterms;
    size_t current_term;
    Term *terms;
    Condition start_condition;
    Signal end_signal;
    bool condition_met;
    void verify_condition(StaticJsonDocument<256> current_state);
    void init(int emulation);
    unsigned long compute_duration(unsigned long duration);
};

#endif /* SRC_CYCLE */
