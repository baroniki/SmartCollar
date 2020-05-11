#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include "accelerometer.h"
#include "gps.h"
#include "temp-sensor.h"
#include "pulse-sensor.h"
#include "float-level.h"
#include "buzzer.h"
#include "led.h"

struct sensor_data {
    struct acc_data acc_data;
    struct gps_data gps_data;
    struct temp_data temp_data;
    struct pulse_data pulse_data;
    struct float_data float_data;
};

#endif