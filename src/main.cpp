#include "Term.h"
#include "Cycle.h"
#include "Manager.h"
#include "config.h"

#define EMULATION_MODE true

Term IDLING_TERMS[] = {
    Term("term 1", Duration(1, 30, 0), Actions{.pump = TOGGLE, .resitance = AUTO}),
    Term("term 2", Duration(10, 35, 0), Actions{.pump = OFF, .resitance = OFF}),
    Term("term 3", Duration(1, 5, 0), Actions{.pump = ON, .resitance = ON}),
};

Term DRAINING_TERMS[] = {
    Term("term 1", Duration(15, 30, 34), Actions{.pump = AUTO, .resitance = AUTO}),
    Term("term 2", Duration(8, 29, 26), Actions{.pump = OFF, .resitance = OFF}),
};

Term V_40_TERMS[] = {
    Term("term 1", Duration(11, 35, 0), Actions{.pump = AUTO, .resitance = AUTO}),
    Term("term 2", Duration(0, 30, 0), Actions{.pump = ON, .resitance = ON}),
    Term("term 3", Duration(0, 45, 0), Actions{.pump = OFF, .resitance = OFF}),
};

Cycle CYCLES[] = {
    Cycle("IDLING", 3, IDLING_TERMS, Condition(max_temperature), Signal{.value = 'A'}),
    Cycle("DRAINING", 2, DRAINING_TERMS),
    Cycle("V-40", 3, V_40_TERMS, Signal{.value = 'B'}),
};

Manager cycleManager(3, CYCLES, EMULATION_MODE);

void setup()
{

  Serial.begin(115200);
  while (!Serial)
    ;

  // Screen pins setup
  Screen.begin(115200);

  while (!Screen)
    ;

  pinPeripheral(5, PIO_SERCOM_ALT); // RX - D5
  pinPeripheral(6, PIO_SERCOM_ALT); // TX - D6
}

void loop()
{
  cycleManager.loop();
}
