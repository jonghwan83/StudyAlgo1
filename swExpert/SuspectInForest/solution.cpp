#define MAXN        (50)
#define MAXD        (11)
#define UP          (0)
#define RIGHT       (1)
#define DOWN        (2)
#define LEFT        (3)

int(*forest)[MAXN];
int forestSize;
int scanRange;
int scanResult[MAXD][MAXD];
int criminalPathCount;
int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

struct Point {
    int r, c;
};

struct CriminalPath {
    int curR;
    int curC;
    int dir;
    bool isRemoved;
    Point pointList[2500];
    int pointCount;

    bool isFirstVisit(int r, int c) {
        for (int i = 0; i < pointCount; i++) {
            if (pointList[i].r == r && pointList[i].c == c)
                return false;
        }
        return true;
    }

    // 죄인이동
    void move() {
        // 바라보는 방향으로 이동했을때의 좌표가 이미 방문한 좌표라면 시계방향으로 90도 회전한다.
        int nextR, nextC;

        while (true) {
            nextR = (forestSize + curR + dr[dir]) % forestSize;
            nextC = (forestSize + curC + dc[dir]) % forestSize;
            if (isFirstVisit(nextR, nextC))
                break;
            dir = (dir + 1) % 4;
        }

        // 바라보는 방향으로 한 칸 이동한다.
        addPoint(nextR, nextC);
    }

    void addPoint(int r, int c) {
        curR = pointList[pointCount].r = r;
        curC = pointList[pointCount].c = c;
        pointCount++;
    }
};

CriminalPath criminalPath[2500 * 4];

extern int useDetector(int P[MAXD][MAXD]);

struct Result
{
    int r, c;
};

// 숲의 p위치(r, c)의 주변 경관이 scanResult와 일치하는지 여부
bool mapcmp(int curR, int curC) {
    int P[MAXD][MAXD]; //숲의 p위치(r, c)의 주변 경관

    for (int r1 = curR - scanRange / 2, r2 = 0; r2 < scanRange; ++r1, ++r2) {
        for (int c1 = curC - scanRange / 2, c2 = 0; c2 < scanRange; ++c1, ++c2) {
            P[r2][c2] = forest[(r1 + forestSize) % forestSize][(c1 + forestSize) % forestSize];

            if (P[r2][c2] != scanResult[r2][c2])
                return false;
        }
    }
    return true;
}

void init(int N, int D, int mForest[MAXN][MAXN], int K)
{
    forest = mForest;
    forestSize = N;
    scanRange = D;
}

Result findCriminal()
{
    criminalPathCount = 0;

    for (int r = 0; r < forestSize; r++) {
        for (int c = 0; c < forestSize; c++) {
            for (int dir = 0; dir < 4; dir++) {
                criminalPath[criminalPathCount].pointCount = 0;
                criminalPath[criminalPathCount].addPoint(r, c);
                criminalPath[criminalPathCount].dir = dir;
                criminalPath[criminalPathCount].isRemoved = false;
                criminalPathCount++;
            }
        }
    }

    int trackingTargetCount = criminalPathCount;

    while (trackingTargetCount != 1) {
        useDetector(scanResult);

        for (int i = 0; i < criminalPathCount; i++) {
            if (criminalPath[i].isRemoved)
                continue;

            criminalPath[i].move();
            // 후보 확인해서 불일치하면 삭제한다.
            if (!mapcmp(criminalPath[i].curR, criminalPath[i].curC)) {
                criminalPath[i].isRemoved = true;
                trackingTargetCount--;
            }
        }
    }

    Result res;
    res.r = res.c = -1;

    for (int i = 0; i < criminalPathCount; i++) {
        if (criminalPath[i].isRemoved)
            continue;
        res.r = criminalPath[i].curR;
        res.c = criminalPath[i].curC;
    }
    return res;
}

