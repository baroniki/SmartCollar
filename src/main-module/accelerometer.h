struct acc_data {
    double x;
    double y;
    double z;
};

struct acc_data read_acc() {
    struct acc_data acc_data;
    acc_data.x = 0;
    acc_data.y = 0;
    acc_data.z = 0;

    return acc_data;
}