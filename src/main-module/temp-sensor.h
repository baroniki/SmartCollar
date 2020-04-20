struct temp_data {
    double temp;
};

struct temp_data read_temp_sensor() {
    struct temp_data temp_data;
    temp_data.temp = 0;

    return temp_data;
}
