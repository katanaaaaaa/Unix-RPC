struct vectors {
    int n;
    int x<>;
    int y<>;
};

struct average_result {
    double avg_x;
    double avg_y;
};

struct scale_input {
    double r;
    int n;
    int x<>;
};

struct scale_result {
    double values<>;
};

program CALC_PROG {
    version CALC_VERS {
        int DOT_PRODUCT(vectors) = 1;
        average_result AVERAGE_VECTORS(vectors) = 2;
        scale_result SCALE_VECTOR(scale_input) = 3;
    } = 1;
} = 0x23452222;
