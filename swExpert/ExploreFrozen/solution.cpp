#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>

using namespace std;

int nrow, ncol;
vector<int> temp;
vector< vector<int> > structureR;   // index: row, value: colums list
vector< vector<int> > structureC;   // index: col, value: row list
vector< vector<int> > visited;

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
    for (int i=0; i < nrow; i++) {
        for (int j=0; j < ncol; j++) {
            visited[i][j] = 0;
        }
    }

    deque< pair<int, int> > q;
    q.push_back(make_pair(sPointR, sPointC));
    visited[sPointR][sPointC] = 1;
    
    int r, c, col_loc, right_idx, left_idx;
    int row_loc, upper_idx, down_idx;
    while (q.size() > 0) {
        r = q.front().first;
        c = q.front().second;
        q.pop_front();

        // horizontal direction
        if (structureR[r].size() > 0) { 
            col_loc = binarySearch(c, structureR[r]);
            // col_loc == len(structureR[r]) - 1, no strucrue on the right
            if (col_loc == structureR[r].size() - 1 && structureR[r][col_loc] < c) {
                // check endpoint
                if (ePointR == r && ePointC > structureR[r][col_loc]) {
                    visited[ePointR][ePointC] = visited[r][c];
                    return;
                }
                if (!visited[r][structureR[r][col_loc] + 1]){
                    visited[r][structureR[r][col_loc] + 1] = visited[r][c] + 1;
                    q.push_back(make_pair(r, structureR[r][col_loc] + 1));
                }        
            }
            // col_loc == 0, no structre on the left
            else if (col_loc == 0 && structureR[r][col_loc] > c) {
                // check endpoint
                if (ePointR == r && ePointC < structureR[r][col_loc]) {
                    visited[ePointR][ePointC] = visited[r][c];
                    return;
                }
                if (!visited[r][structureR[r][col_loc] - 1]) {
                    visited[r][structureR[r][col_loc] - 1] = visited[r][c] + 1;
                    q.push_back(make_pair(r, structureR[r][col_loc] - 1));
                }
            }

            else {
                right_idx = structureR[r][col_loc];
                left_idx = structureR[r][col_loc - 1];
                // check endpoint
                if (ePointR == r && left_idx <= ePointC && ePointC <= right_idx) {
                    visited[ePointR][ePointC] = visited[r][c];
                    return;
                }
                else {
                    // left side
                    if (!visited[r][left_idx + 1]) {
                        visited[r][left_idx + 1] = visited[r][c] + 1;
                        q.push_back(make_pair(r, left_idx + 1));
                    }
                    // right side
                    if (!visited[r][right_idx - 1]) {
                        visited[r][right_idx - 1] = visited[r][c] + 1;
                        q.push_back(make_pair(r, right_idx - 1));
                    }
                }
            }
        }
        else {
            if (ePointR == r) { 
                visited[ePointR][ePointC] = visited[r][c];
                return;
            }
        }

        if (structureC[c].size() > 0) {
        // vertical direction
            row_loc = binarySearch(r, structureC[c]);
            // row_loc == len(structureC[c]) - 1 , no structure on down side
            if (row_loc == structureC[c].size() - 1 && structureC[c][row_loc] < r) {
                if (ePointC == c && structureC[c][row_loc] < ePointR) {
                    visited[ePointR][ePointC] = visited[r][c];
                    return;
                }
                if (!visited[structureC[c][row_loc] + 1][c]) {
                    visited[structureC[c][row_loc] + 1][c] = visited[r][c] + 1;
                    q.push_back(make_pair(structureC[c][row_loc] + 1, c));
                }
            }
            // no structure on upper side
            else if (row_loc == 0 && structureC[c][row_loc] > r) {
                if (ePointC == c && structureC[c][row_loc] > ePointR) {
                    visited[ePointR][ePointC] = visited[r][c];
                    return;
                }
                if (!visited[structureC[c][row_loc]-1][c]) {
                    visited[structureC[c][row_loc]-1][c] = visited[r][c] + 1;
                    q.push_back(make_pair(structureC[c][row_loc] - 1, c));
                }
            }
            else {
                upper_idx = structureC[c][row_loc-1];
                down_idx = structureC[c][row_loc];
                // check end point
                if (ePointC == c && upper_idx <= ePointR && ePointR <= down_idx) {
                    visited[ePointR][ePointC] = visited[r][c];
                    return;
                }
                // upper side
                if (!visited[upper_idx + 1][c]) {
                    visited[upper_idx + 1][c] = visited[r][c] + 1;
                    q.push_back(make_pair(upper_idx + 1, c));
                }
                // down side
                if (!visited[down_idx - 1][c]) {
                    visited[down_idx - 1][c] = visited[r][c] + 1;
                    q.push_back(make_pair(down_idx - 1, c));
                }
            }
        }

        else {
            if (ePointC == c) {
                visited[ePointR][ePointC] = visited[r][c];
                return;
            }
        }
    }
    return;
}

void init(int R, int C, int M, int mStructureR[30000], int mStructureC[30000])
{
    visited.clear();
    structureR.clear();
    structureC.clear();
    vector<int> zeros;

    nrow = R;
    ncol = C;
    
    temp.clear();

    for (int i=0; i < C; i++) {
        structureC.push_back(temp);
        zeros.push_back(0);
    }
    for (int i=0; i < R; i++) {
        structureR.push_back(temp);
        visited.push_back(zeros);
    }

    for (int i=0; i < M; i++) {
        structureR[mStructureR[i]].push_back(mStructureC[i]);
        structureC[mStructureC[i]].push_back(mStructureR[i]);
    }

    for (int i=0; i < structureR.size(); i++) {
        if (structureR[i].size() > 0) {
            sort(structureR[i].begin(), structureR[i].end());
        }
    }

    for (int i=0; i < structureC.size(); i++) {
        if (structureC[i].size() > 0) {
            sort(structureC[i].begin(), structureC[i].end());
        }
    }
}

int minDamage(int mStartR, int mStartC, int mEndR, int mEndC)
{

    bfs(mStartR, mStartC, mEndR, mEndC);
    return visited[mEndR][mEndC] - 1;
}