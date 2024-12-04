#ifndef LOGGER_H
#define LOGGER_H

#ifdef ARDUINO

#include <Arduino.h>

#define print(l) Serial.println(l)
#define println(l) Serial.println(l)

#else

#include <stdio.h>

#define print(line) printf(line)
#define println(line) printf(line"\n")

#endif

#endif