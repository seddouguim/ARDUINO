#include "State.h"
#include "Manager.h"
#include "config.h"

State::State()
    : manager(nullptr), thermocouple(Adafruit_MAX31855(MAX31855CLK, MAX31855CS, MAX31855DO))
{
    this->initialized = false;
    this->last_update = 0;
    this->resistance_state = ON;
    this->previous_resistance_state = ON;
    this->pump_state = OFF;
    this->previous_pump_state = OFF;
    this->temperature = 0;
    this->toggle_pump_start_time = 0ul;
}

void State::bindManager(Manager *manager)
{
    this->manager = manager;
}

void State::init()
{
    Serial.println("Initializing thermocouple...");
    this->thermocouple.begin();
    delay(500);

    pinMode(RESISTANCE_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);

    Serial.println("Initializing thermocouple...done");

    // Initial state is R: ON, P: OFF
    digitalWrite(RESISTANCE_PIN, HIGH);
    digitalWrite(PUMP_PIN, LOW);

    this->initialized = true;
}

StaticJsonDocument<256> State::getState()
{
    StaticJsonDocument<256> data;
    data["last_energy"] = 0;
    data["current_energy"] = 0;
    data["last_temperature"] = this->previous_temperature;
    data["current_temperature"] = this->temperature;
    data["resistance_status"] = this->resistance_state;
    data["pump_status"] = this->pump_state;
    return data;
}

void State::update(Actions actions, bool cycle_running)
{
    if (!initialized)
        init();

    this->previous_pump_state = this->pump_state;
    this->previous_resistance_state = this->resistance_state;
    this->previous_temperature = this->temperature;

    // Read the temperature from the thermocouple
    // this->temperature = this->thermocouple.readCelsius();
    this->temperature = temperature + 5;

    this->resistance_state = digitalRead(RESISTANCE_PIN);
    this->pump_state = digitalRead(PUMP_PIN);

    // only if the cycle is running to avoid running before condition is met
    if (cycle_running)
        performActions(actions);
}

void State::performActions(Actions actions)
{
    handleResistance(actions);
    handlePump(actions);
}

void State::handleResistance(Actions actions)
{
    if (actions.resitance != resistance_state)
    {
        switch (actions.resitance)
        {
        case ON:
            digitalWrite(RESISTANCE_PIN, HIGH);
            Serial.println("Resistance ON.");
            break;

        case OFF:
            digitalWrite(RESISTANCE_PIN, LOW);
            Serial.println("Resistance OFF.");
            break;

        case AUTO:
            if (temperature >= max_temperature && resistance_state == ON)
            {
                digitalWrite(RESISTANCE_PIN, LOW);
                Serial.println("Temperature " + String(temperature) + " > " + String(max_temperature) + " -> Resistance OFF.");
            }
            else if (temperature <= min_temperature && resistance_state == OFF)
            {
                digitalWrite(RESISTANCE_PIN, HIGH);
                Serial.println("Temperature " + String(temperature) + " < " + String(min_temperature) + " -> Resistance ON.");
            }
            break;
        }
    }
}

void State::handlePump(Actions actions)
{
    if (actions.pump != pump_state)
    {
        switch (actions.pump)
        {
        case ON:
            digitalWrite(PUMP_PIN, HIGH);
            Serial.println("Pump ON.");
            break;

        case OFF:
            digitalWrite(PUMP_PIN, LOW);
            Serial.println("Pump OFF.");
            break;

        case TOGGLE:
            // turn pump on for 20 seconds and turn off for 40 seconds
            if (toggle_pump_start_time == 0ul)
                toggle_pump_start_time = millis();

            if (pump_state == OFF && millis() == toggle_pump_start_time)
            {
                digitalWrite(PUMP_PIN, HIGH);
                Serial.println("Pump ON for 20 seconds.");
            }
            if (millis() - toggle_pump_start_time > 20000 && pump_state == ON)
            {
                digitalWrite(PUMP_PIN, LOW);
                Serial.println("Pump OFF for 40 seconds.");
            }

            if (millis() - toggle_pump_start_time > 60000)
                toggle_pump_start_time = 0ul;
            break;

        case AUTO:
            if (temperature >= max_temperature && pump_state == ON)
            {
                digitalWrite(PUMP_PIN, LOW);
                Serial.println("Temperature " + String(temperature) + " > " + String(max_temperature) + " -> Pump OFF.");
            }
            else if (temperature <= min_temperature && pump_state == OFF)
            {
                digitalWrite(PUMP_PIN, HIGH);
                Serial.println("Temperature " + String(temperature) + " < " + String(min_temperature) + " -> Pump ON.");
            }
            break;
        }
    }
}
