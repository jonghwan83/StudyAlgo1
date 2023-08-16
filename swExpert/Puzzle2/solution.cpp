
extern void move(int mode);

#define U move(0)
#define D move(1)
#define L move(2)
#define R move(3)

const int (*p)[3];

void goCenter() {
    if (p[0][0] == 0) { D; R; }
    else if (p[0][1] == 0) { D; }
    else if (p[0][2] == 0) { D; L; }

    else if (p[1][0] == 0) { R; }
    else if (p[1][2] == 0) { L; }

    else if (p[2][0] == 0) { R; U; }
    else if (p[2][1] == 0) { U; }
    else if (p[2][2] == 0) { L; U; }
}

void make1() {
    if (p[0][1] == 1) { L; U; R; }
    else if (p[0][2] == 1) { U; R; D; L; L; U; R; }
    else if (p[1][0] == 1) { U; L; D; }
    else if (p[1][2] == 1) { U; R; D; L; U; R; D; L; L; U; R; }
    else if (p[2][0] == 1) { L; D; R; U; U; L; D; }
    else if (p[2][1] == 1) { L; D; R; U; U; L; D; D; R; U; U; L; D; }
    else if (p[2][2] == 1) { D; R; U; L; L; D; R; U; U; L; D; D; R; U; U; L; D; }

    goCenter();
}

void make2() {
    if (p[0][1] == 2) { R; U; L; }
    else if (p[1][0] == 2) { L; U; R; D; R; U; L; L; D; }
    else if (p[1][2] == 2) { U; R; D; }
    else if (p[2][0] == 2) { L; D; R; U; L; U; R; D; R; U; L; L; D; }
    else if (p[2][1] == 2) { L; D; R; U; L; D; R; U; L; U; R; D; R; U; L; L; D; }
    else if (p[2][2] == 2) { R; D; L; U; U; R; D; }

    goCenter();
}

void make3() {
    if (p[1][0] == 3) { L; D; R; R; U; L; }
    else if (p[2][0] == 3) { D; L; U; R; D; R; U; L; }
    else if (p[2][1] == 3) { D; R; U; L; }
    else if (p[2][2] == 3) { R; D; }
    else if (p[0][1] == 3) { U; R; D; D; L; U; R; U; L; D; D; R; U; L; }

    goCenter();

    U; R; D; D;
}


void clockwise(bool isLeft) {
    if (isLeft) { L; }
    L; U; R; D;
    if (isLeft) { R; }
}

void make6() {
    if (p[2][1] == 6) { U; L; D; L; U; R; R; D; }

    while (p[1][1] != 6) {
        clockwise(true);
    }

    U; L; D;
}


void test(const int P[3][3]) {
    p = P;

    goCenter();

    make1();

    make2();

    make3();

    while (p[1][2] != 8) {
        if (p[1][0] == 8 || p[2][0] == 8) {
            clockwise(true);
        }

        clockwise(false);
    }

    make6();

    while (p[1][0] != 4) {
        clockwise(false);
    }

    R;
}