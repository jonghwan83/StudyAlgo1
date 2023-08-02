
extern void scan(int floorState[3][3]);

extern int move(void);

extern void turn(int mCommand);


#include <iostream>

#define MAXSIZE 64

int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, -1, 0, 1 };

int unscanned, uncleaned, cleaned, wall;

int house[MAXSIZE][MAXSIZE];
int visited[MAXSIZE][MAXSIZE];

int nVisited;

void showHouse() {

	for (int i = 0; i < MAXSIZE; i++) {
		for (int j = 0; j < MAXSIZE; j++) {
			printf("%d ", house[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


class Robot {
public:
	int row;
	int col;
	int dir;

	void init() {
		row = 1; col = 1; dir = 0;
		house[row][col] = cleaned;
	}
};


class Queue {
public:
	int length, st, ed;
	Robot arr[MAXSIZE * MAXSIZE];

	void init() {
		length = 0; st = 0; ed = 0;
	}

	void push(int r, int c, int d) {
		length++;
		arr[ed].row = r; arr[ed].col = c; arr[ed].dir = d;
		ed++;
	}

	Robot pop() {
		length--;
		return arr[st++];
	}
};


Robot robotInfo;

int convert(int a) {
	if (a < 0) { return a + 64; }
	if (a >= 64) { return a - 64; }
	return a;
}


void update(int a[3][3]) {

	int rev[3] = { 2, 1, 0 };

	int temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			if (robotInfo.dir == 1) {
				temp[i][j] = a[j][rev[i]];
			}
			else if (robotInfo.dir == 2) {
				temp[i][j] = a[rev[i]][rev[j]];
			}
			else if (robotInfo.dir == 3) {
				temp[i][j] = a[rev[j]][i];
			}
			else {
				temp[i][j] = a[i][j];
			}
		}
	}

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			int dr = convert(robotInfo.row + i);
			int dc = convert(robotInfo.col + j);

			if (house[dr][dc] <= unscanned && temp[i + 1][j + 1] == 1) {
				house[convert(i + robotInfo.row)][convert(j + robotInfo.col)] = wall;
			}
			else if (house[dr][dc] <= unscanned && temp[i + 1][j + 1] == 0) {
				house[convert(i + robotInfo.row)][convert(j + robotInfo.col)] = uncleaned;
			}

		}
	}

	return;
}


void scan_norm() {
	int around[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			int dr = convert(robotInfo.row + i - 1);
			int dc = convert(robotInfo.col + j - 1);

			if (house[dr][dc] <= unscanned) {
				scan(around);
				update(around);
			}
		}
	}

}

void turn_norm(int dest) {

	int k = dest - robotInfo.dir;
	if (k < 0) { k += 4; }
	if (k >= 4) { k -= 4; }

	if (k == 0) { return; }

	turn(k);
	robotInfo.dir = dest;
}

bool move_norm(bool isForced) {

	int dr = convert(robotInfo.row + drows[robotInfo.dir]);
	int dc = convert(robotInfo.col + dcols[robotInfo.dir]);

	if (house[dr][dc] <= unscanned && !isForced) {
		scan_norm();
	}


	if (house[dr][dc] == uncleaned || isForced) {
		move();
		robotInfo.row = dr;
		robotInfo.col = dc;
		house[dr][dc] = cleaned;

		return true;
	}

	return false;
}

void go() {

	while (true) {

		bool result = move_norm(false);

		if (!result) { break; }
	}

}



int bfs() {
	nVisited++;

	Queue queue; queue.init();

	queue.push(robotInfo.row, robotInfo.col, -1);
	visited[robotInfo.row][robotInfo.col] = nVisited;

	while (queue.length > 0) {
		Robot curr = queue.pop();

		for (int i = 0; i < 4; i++) {
			int dr = convert(curr.row + drows[i]);
			int dc = convert(curr.col + dcols[i]);

			if (visited[dr][dc] >= nVisited) { continue; }
			if (house[dr][dc] == wall) { continue; }

			if (curr.dir == -1) {
				queue.push(dr, dc, i);
			}
			else {
				queue.push(dr, dc, curr.dir);
			}

			visited[dr][dc] = nVisited;

			if (house[dr][dc] == uncleaned || house[dr][dc] <= unscanned) {
				if (curr.dir == -1) { return i; }
				else { return curr.dir; }
			}
		}
	}

	return -1;
}



void init(void)
{
	unscanned = -3;
	uncleaned = -2;
	wall = -1;
	cleaned = 0;

	for (int i = 0; i < MAXSIZE; i++) {
		for (int j = 0; j < MAXSIZE; j++) {
			house[i][j] = 0;
			visited[i][j] = 0;
		}
	}

	robotInfo.init();
	nVisited = 0;

	return;
}



void cleanHouse(void)

{
	unscanned += 3;
	uncleaned += 3;
	wall += 3;
	cleaned += 3;


	while (true) {

		go();

		scan_norm();

		for (int i = 0; i < 4; i++) {
			int dr = convert(robotInfo.row + drows[i]);
			int dc = convert(robotInfo.col + dcols[i]);

			if (house[dr][dc] == uncleaned) {
				turn_norm(i);
				break;
			}

			if (i == 3) {
				int nDir = bfs();
				if (nDir == -1) {
					//showHouse();
					return;
				}

				turn_norm(nDir);
				move_norm(true);
			}
		}

	}

	return;
}
