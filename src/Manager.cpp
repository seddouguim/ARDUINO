#include "Manager.h"
#include "utils.h"

Manager::Manager(size_t ncycles, Cycle *cycles, int emulation)
{
    this->ncycles = ncycles;
    this->current_cycle = 0;
    this->is_running = true;
    this->cycles = cycles;
    this->last_loop = 0;
    this->initialized = false;
    this->network = Network();
    this->state = State();
    this->display = Display();
    this->EMULATION_MODE = emulation;
    this->state.bindManager(this);
}

Actions Manager::getActions()
{
    return cycles[current_cycle].getActions();
}

String Manager::get_current_cycle()
{
    return String(current_cycle) + String(".") + String(cycles[current_cycle].get_current_term());
}

void Manager::init()
{
    Serial.println("Initializing buzzer...");
    pinMode(BUZZER_PIN, OUTPUT);
    initialized = true;
}

void Manager::loop()
{
    if (!initialized)
        init();

    // State - deals with pump and resistance. Returns JSON object of state to send to server.
    state.update(getActions(), cycles[current_cycle].is_running());

    // Network - Connects to wifi and sends data to server (data received from state)
    network.run(state.getState());

    // Display
    display.run(state.getState());

    if (!is_running)
        return;

    // Cycle - deals with the actual cycle. Deals with terms. Cycle returns 0 if cycle is finished.
    // Needs state to verify start condition.
    if (cycles[current_cycle].run(EMULATION_MODE, state.getState()) == 0)
    {
        current_cycle = current_cycle >= ncycles - 1 ? is_running = false : current_cycle + 1;
        Buzzer(300, 0, 1);
    }
}
