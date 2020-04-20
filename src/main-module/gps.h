struct gps_data {
    double latitude;
    double longitude;
};

struct gps_data read_gps() {
    struct gps_data gps_data;
    gps_data.latitude = 0;
    gps_data.longitude = 0;

    return gps_data;
}