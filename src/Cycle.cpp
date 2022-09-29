#include "Cycle.h"

Cycle::Cycle(String name, size_t nterms, Term *terms, Condition start_condition, Signal end_signal)
{
    this->name = name;
    isRunning = 0;
    this->nterms = nterms;
    this->current_term = 0;
    this->terms = terms;
    this->start_condition = start_condition;
    this->duration = compute_duration(0);
    this->condition_met = false;
    this->end_signal = end_signal;
}

Cycle::Cycle(String name, size_t nterms, Term *terms, Signal end_signal)
{
    this->name = name;
    isRunning = 0;
    this->nterms = nterms;
    this->current_term = 0;
    this->terms = terms;
    this->start_condition = Condition();
    this->duration = compute_duration(0);
    this->condition_met = false;
    this->end_signal = end_signal;
}

Cycle::Cycle(String name, size_t nterms, Term *terms)
{
    this->name = name;
    isRunning = 0;
    this->nterms = nterms;
    this->current_term = 0;
    this->terms = terms;
    this->start_condition = Condition();
    this->duration = compute_duration(0);
    this->condition_met = false;
    this->end_signal = Signal();
}

size_t Cycle::get_current_term()
{
    return current_term;
}

Actions Cycle::getActions()
{
    return terms[current_term].getActions();
}

unsigned long Cycle::compute_duration(unsigned long duration)
{
    if (duration != 0)
        return duration;

    unsigned long sum = 0;
    for (size_t i = 0; i < nterms; i++)
    {
        sum += terms[i].getDuration();
    }
    return sum;
}

bool Cycle::is_running()
{
    return isRunning;
}

void Cycle::verify_condition(StaticJsonDocument<256> current_state)
{
    if (start_condition.resistance_state == NIL &&
        start_condition.pump_state == NIL &&
        start_condition.temperature == NIL)
    {
        Serial.println(F("Start condition not set. Starting cycle."));
        condition_met = true;
        return;
    }
    else if (((current_state["current_temperature"] >= start_condition.temperature) ||
              start_condition.temperature == NIL) &&
             ((current_state["resistance_state"] == start_condition.resistance_state) ||
              start_condition.resistance_state == NIL) &&
             ((current_state["pump_state"] == start_condition.pump_state) ||
              start_condition.pump_state == NIL))
    {
        condition_met = true;
        Serial.println();
        Serial.println("Cycle " + name + ": start condition met.");
    }
}

void Cycle::init(int emulation)
{
    // UNO SERIAL
    if (end_signal.value != '\0')
    {
        Serial1.begin(115200);

        while (!Serial1)
            ;
    }

    t_duration = emulation ? duration / EMULATION_DURATION : duration;
    end_time = millis() + t_duration;

    Serial.println("***** Cycle " + name + " started." + " Duration: " + Duration::toString(t_duration) + " *****");
    isRunning = 1;
}

int Cycle::run(int emulation, StaticJsonDocument<256> current_state)
{

    if (!condition_met)
    {
        verify_condition(current_state);
        return -1;
    }

    if (isRunning == 0)
        init(emulation);

    if (!terms[current_term].run(emulation))
    {
        current_term = current_term >= nterms - 1 ? isRunning = false : current_term + 1;
    }

    if (end_time <= millis())
    {
        // SENDING END SIGNAL TO UNO
        if (end_signal.value != '\0')
            Serial1.write(end_signal.value);

        Serial.println("Cycle " + name + " finished.");
        isRunning = 0;
    }

    return isRunning;
}
