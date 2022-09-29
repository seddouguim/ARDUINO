#ifndef SRC_CONFIG
#define SRC_CONFIG

#include <Arduino.h>
#include "wiring_private.h"

#define EMULATION_DURATION 500

// ARDUINO PINS
#define BUZZER_PIN 2
#define PUMP_PIN 3
#define RESISTANCE_PIN 4

#define MAX31855CLK 8
#define MAX31855CS 9
#define MAX31855DO 10

// ESP PINS
// #define BUZZER_PIN 13
// #define RESISTANCE_PIN 5
// #define PUMP_PIN 4

// #define CLK 14
// #define CS 15
// #define MISO 12

extern float min_temperature;
extern float max_temperature;

extern float PON;
extern float POFF;

extern Uart Screen;

#define GREEN 960
#define RED 47169
#define BLUE 343

#define RESISTANCE_OFF_PIC 81
#define PUMP_OFF_PIC 80

#endif /* SRC_CONFIG */
