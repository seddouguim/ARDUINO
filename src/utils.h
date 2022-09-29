#ifndef SRC_UTILS
#define SRC_UTILS

#include "config.h"
#include "ArduinoJson.h"

#define OFF 0
#define ON 1
#define AUTO 2
#define TOGGLE 3
#define NIL -7

typedef struct Duration
{
    int hours;
    int minutes;
    int seconds;
    Duration(
        int h = 0, int m = 0, int s = 0, void (*callback)(void) = NULL)
        : hours(h), minutes(m), seconds(s)
    {
        if (callback != NULL)
            callback();
    }
    unsigned long toMillis()
    {
        return hours * 3600 * 1000 + minutes * 60 * 1000 + seconds * 1000;
    }

    String static toString(unsigned long duration)
    {
        // duration string in hh::mm::ss format
        String duration_str = "";
        int hours = duration / (3600 * 1000);
        int minutes = (duration % (3600 * 1000)) / (60 * 1000);
        int seconds = (duration % (60 * 1000)) / 1000;

        if (hours > 0)
            duration_str += String(hours) + ":";

        if (minutes < 10)
            duration_str += "0";
        duration_str += String(minutes) + ":";

        if (seconds < 10)
            duration_str += "0";
        duration_str += String(seconds);

        return duration_str;
    }
} Duration;

typedef struct Actions
{
    int pump;
    int resitance;
} Actions;

typedef struct Condition
{
    float temperature;
    int resistance_state;
    int pump_state;

    Condition(float t = NIL, int r = NIL, float p = NIL)
        : temperature(t), resistance_state(r), pump_state(p) {}
} Condition;

typedef struct Signal
{
    char value;
} Signal;

void Buzzer(int TempsH, int TempsL, int nb);

bool same_state(StaticJsonDocument<256> current_state, StaticJsonDocument<256> previous_state);

#endif /* SRC_UTILS */
