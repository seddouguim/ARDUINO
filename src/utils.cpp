#include "Arduino.h"
#include "utils.h"

void Buzzer(int TempsH, int TempsL, int nb)
{
    for (int x = 1; x <= nb; x++)
    {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(TempsH);
        digitalWrite(BUZZER_PIN, LOW);
        delay(TempsL);
    }
}

bool same_state(StaticJsonDocument<256> current_state, StaticJsonDocument<256> previous_state)
{
    return (current_state == previous_state);
}