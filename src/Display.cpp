#include "Display.h"

Display::Display()
{
    this->initialized = false;
}

void Display::init()
{
    Screen.print("page0.tempDisplay.txt=\"" + String("99.0") + "\"");
    Screen.write(0xff);
    Screen.write(0xff);
    Screen.write(0xff);
    this->initialized = true;
}

void Display::run(StaticJsonDocument<256> current_state)
{
    if (!initialized)
        init();

    // update
    update(current_state);
}

void Display::update(StaticJsonDocument<256> current_state)
{
    if (previous_state == current_state)
        return;

    update_temperature(current_state["current_temperature"]);
    update_resistance(current_state["resistance_status"]);
    update_pump(current_state["pump_status"]);

    previous_state = current_state;
}

void Display::update_pump(int pump_state)
{
    if ((int)pump_state == (int)previous_state["pump_status"])
        return;

    if (pump_state == ON)
    {
        Screen.print("page0.pumpTimer.en=1");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.pumpStatus.txt=\"ON\"");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.pumpStatus.pco=" + String(GREEN));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);
    }

    else
    {
        Screen.print("page0.pumpTimer.en=0");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.pump.pic=" + String(PUMP_OFF_PIC));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.pumpStatus.txt=\"OFF\"");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.pumpStatus.pco=" + String(RED));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);
    }
}

void Display::update_resistance(int resistance_state)
{
    if ((int)resistance_state == (int)previous_state["resistance_status"])
        return;

    if (resistance_state == ON)
    {
        Screen.print("page0.resTimer.en=1");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.resStatus.txt=\"ON\"");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.resStatus.pco=" + String(GREEN));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);
    }

    else
    {
        Screen.print("page0.resTimer.en=0");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.fire.pic=" + String(RESISTANCE_OFF_PIC));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.resStatus.txt=\"OFF\"");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("page0.resStatus.pco=" + String(RED));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);
    }
}

void Display::update_temperature(float temperature)
{

    if ((float)temperature != (float)previous_state["current_temperature"])
    {
        float previous_temperature = previous_state["current_temperature"];
        float trend = temperature - previous_temperature;

        Screen.print("page0.tempDisplay.txt=\"" + String(temperature, 1) + char(176) + "\"");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        if (trend > 0)
        {
            Screen.print("page0.tempDisplay.pco=" + String(RED));
            Screen.write(0xff);
            Screen.write(0xff);
            Screen.write(0xff);

            Screen.print("page0.arrow.pic=9");
            Screen.write(0xff);
            Screen.write(0xff);
            Screen.write(0xff);

            Screen.print("vis page0.arrow,1");
            Screen.write(0xff);
            Screen.write(0xff);
            Screen.write(0xff);
        }

        else if (trend < 0)
        {
            Screen.print("page0.tempDisplay.pco=" + String(BLUE));
            Screen.write(0xff);
            Screen.write(0xff);
            Screen.write(0xff);

            Screen.print("page0.arrow.pic=8");
            Screen.write(0xff);
            Screen.write(0xff);
            Screen.write(0xff);

            Screen.print("vis page0.arrow,1");
            Screen.write(0xff);
            Screen.write(0xff);
            Screen.write(0xff);
        }
    }

    else
    {
        Screen.print("page0.tempDisplay.pco=" + String(GREEN));
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);

        Screen.print("vis page0.arrow,0");
        Screen.write(0xff);
        Screen.write(0xff);
        Screen.write(0xff);
    }
}