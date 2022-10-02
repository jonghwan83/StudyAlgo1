#include <vector>
#include <deque>
#include <iostream>

using namespace std;

int nrow, ncol;
vector<int> temp;
vector< vector<int> > structureR;   // index: row, value: colums list
vector< vector<int> > structureC;   // index: col, value: row list

int binarySearch(int target, vector<int> arr) {
    int left, right, mid;
    left = 0;
    right = arr.size() - 1;

    while (right > left) {
        mid = (right + left) / 2;
        if (target > arr[mid]) { 
            // target is on the right
            left = mid + 1;
        } else {
            // target is on the left
            right = mid;
        }
    }
    
    return right;
}

void bfs(int sPointR, int sPointC, int ePointR, int ePointC) {
    vector< vector<int> > visited;
    for (int i=0; i < nrow; i++) {
        temp.clear;
        for (int j=0; j < ncol; j++) {
            temp.push_back(0);
        }
        visited[i].push_back(temp);
    }
    
    deque< pair<int, int> > q;
    q.push_back(make_pair(sPointR, sPointC));

    pair<int, int> cPoint;
    int r, c, col_loc;
    while (q.size() > 0) {
        r = q.front().first;
        c = q.front().second;
        q.pop_front();

        // horizontal direction
         col_loc = binarySearch(c, structureR[r]);
        
        

        // vertical direction
    }

    return;
}

void init(int R, int C, int M, int mStructureR[30000], int mStructureC[30000])
{
    nrow = R;
    ncol = C;
    
    for (int i=0; i < R; i++) {
        structureR.push_back(temp);
    }
    for (int i=0; i < C; i++) {
        structureC.push_back(temp);
    }

    for (int i=0; i < M; i++) {
        structureR[mStructureR[i]].push_back(mStructureC[i]);
        structureC[mStructureC[i]].push_back(mStructureR[i]);
    }

    for (int i=0; i < R; i++) {
        for (int j=0; j < structureR[i].size(); j++) {
            cout << structureR[i][j] << " ";
        }
        cout << endl;
    }
}

int minDamage(int mStartR, int mStartC, int mEndR, int mEndC)
{
 return 0;
}