#include "Term.h"

Term::Term(String name, Duration duration, Actions actions)
{
    this->name = name;
    this->duration = duration.toMillis();
    isRunning = false;
    this->actions = actions;
}

bool Term::run(int emulation)
{
    if (!isRunning)
    {
        isRunning = true;
        t_duration = emulation ? duration / EMULATION_DURATION : duration;
        end_time = millis() + t_duration;
        Serial.println("Term " + name + " started... Duration: " + Duration::toString(t_duration));
    }

    if (end_time <= millis())
    {
        isRunning = false;
        Serial.println("Term " + name + " ended.");
    }

    return isRunning;
}

Actions Term::getActions()
{
    return actions;
}

unsigned long Term::getDuration()
{
    return duration;
}