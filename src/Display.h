#ifndef SRC_DISPLAY
#define SRC_DISPLAY

#include "ArduinoJson.h"
#include "config.h"
#include "utils.h"

class Display
{
public:
    Display();
    void run(StaticJsonDocument<256> current_state);

private:
    StaticJsonDocument<256> previous_state;
    bool initialized;
    void init();
    void update(StaticJsonDocument<256> current_state);
    void update_temperature(float temperature);
    void update_resistance(int resistance_state);
    void update_pump(int pump_state);
};

#endif /* SRC_DISPLAY */
