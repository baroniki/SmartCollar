struct pulse_data {
    double pulse;
};

struct pulse_data read_pulse_sensor() {
    struct pulse_data pulse_data;
    pulse_data.pulse = 0;

    return pulse_data;
}