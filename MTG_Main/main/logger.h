#ifndef LOGGER_H
#define LOGGER_H

#include "config.h"

#ifdef ARDUINO

#include <Arduino.h>
#include <stdio.h>
#define printf(fmt, args...) {char str[100]; snprintf(&str[0], 100, ftm, ## args); Serial.print(&str[0]);}

#else

#include <stdio.h>

#endif // ARDUINO

#ifndef LOGLEVEL
#define LOGLEVEL WARN
#endif

#define LOG_C(fmt, args...) printf("\n\n\n!!!!!!!!!! CRITICAL ERROR !!!!!!!!!!!!!!\n\n\tfile: %s:%3u\n\tmsg: " fmt "\n\n", __FILE__, __LINE__, ## args)

#if LOGLEVEL != "CRITICAL"
#define LOG_E(fmt, args...) printf("ERROR %30s:%3u  " fmt "\n", __FILE__, __LINE__, ## args)

#if LOGLEVEL != "ERROR"
#define LOG_W(fmt, args...) printf("WARN  %30s:%3u  " fmt "\n", __FILE__, __LINE__, ## args)

#if LOGLEVEL != "WARN"
#define LOG_I(fmt, args...) printf("INFO  %30s:%3u  " fmt "\n", __FILE__, __LINE__, ## args)

#if LOGLEVEL != "INFO"
#define LOG_D(fmt, args...) printf("DEBUG %30s:%3u  " fmt "\n", __FILE__, __LINE__, ## args)
#else
#define LOG_D(fmt, args...)
#endif // INFO

#else
#define LOG_I(fmt, args...)
#endif // WARN

#else
#define LOG_W(fmt, args...)
#endif // ERROR

#else
#define LOG_E(fmt, args...)
#endif // CRITICAL


#endif // LOGGER_H
