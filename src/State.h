#ifndef SRC_STATE
#define SRC_STATE

#include "Adafruit_MAX31855.h"
#include "ArduinoJson.h"
#include "utils.h"

class Manager;

class State
{
public:
    State();
    void bindManager(Manager *manager);
    void update(Actions actions, bool cycle_running);
    StaticJsonDocument<256> getState();

private:
    Manager *manager;
    Adafruit_MAX31855 thermocouple;
    float temperature;
    float previous_temperature;
    int resistance_state;
    int previous_resistance_state;
    int pump_state;
    int previous_pump_state;
    bool initialized;
    unsigned long last_update;
    unsigned long toggle_pump_start_time;
    void performActions(Actions actions);
    void handleResistance(Actions actions);
    void handlePump(Actions actions);
    void init();
};

#endif /* SRC_STATE */
