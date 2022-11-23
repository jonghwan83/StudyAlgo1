#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>

#define INF 1e9
#define MAXCITIES 3001
#define NUMKEY 8

using namespace std;

int totalCities;
int limitDistance;
vector< vector< vector<int> > > recommendCities(NUMKEY);
vector<int> centers(3);
vector< vector<int> > grid(MAXCITIES, vector<int>(MAXCITIES));
vector< vector<int> > network(MAXCITIES);
vector<int> prices(MAXCITIES);

int center2key(int mOpt, vector<int> mDestinations) {
    int ans = 0;
    vector<int> check(3);

    for (int i = 0; i < mOpt; i++) {
        for (int j = 0; j < 3; j++) {
            if (mDestinations[i] == centers[j]) {
                check[j] = 1;
                break;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        ans += (check[i] << i);
    }
    return ans;
}

bool checkCenter(int city) {
    for (int i = 0; i < 3; i++) {
        if (city == centers[i]) { return true; }
    }
    return false;
}

vector<int> bfs(int start) {
    deque<int> queue;
    vector<int> visited(totalCities);
    for (int i = 0; i < totalCities; i++) {
        visited[i] = INF;
    }

    queue.push_back(start);
    visited[start] = 0;

    int node;
    while (!queue.empty()) {
        node = queue.front();
        queue.pop_front();

        for (int next : network[node]) {
            if (grid[node][next] + visited[node] < visited[next]) {
                visited[next] = grid[node][next] + visited[node];
                queue.push_back(next);
            }
        }
    }
    return visited;
}

void init(int N, int mDownTown[])
{
    totalCities = N + 1;
    for (int i = 0; i < 3; i++) {
        centers[i] = mDownTown[i];
    }

    for (int i = 0; i < NUMKEY; i++) {
        recommendCities[i].clear();
    }

    for (int i = 0; i < totalCities; i++) {
        network[i].clear();
        prices[i] = 0;
        for (int j = 0; j < totalCities; j++) {
            grid[i][j] = 10;
        }
    }
}

void newLine(int M, int mCityIDs[], int mDistances[])
{
    for (int i = 0; i < (M - 1); i++) {
        if (grid[mCityIDs[i]][mCityIDs[i + 1]] == 10) {
            network[mCityIDs[i]].push_back(mCityIDs[i + 1]);
            network[mCityIDs[i + 1]].push_back(mCityIDs[i]);
        }
        grid[mCityIDs[i]][mCityIDs[i + 1]] = min(grid[mCityIDs[i]][mCityIDs[i + 1]], mDistances[i]);
        grid[mCityIDs[i + 1]][mCityIDs[i]] = min(grid[mCityIDs[i + 1]][mCityIDs[i]], mDistances[i]);
    }

    for (int i = 0; i < NUMKEY; i++) {
        recommendCities[i].clear();
    }
}
void changeLimitDistance(int mLimitDistance)
{
    limitDistance = mLimitDistance;
    vector< vector<int> > minWeight;
    for (int i = 0; i < 3; i++) {
        minWeight.push_back(bfs(centers[i]));
    }

    int key;
    vector< vector<int> > select2 = { {0, 1}, {0, 2}, {1, 2} };
    for (int i = 1; i < totalCities; i++) {
        if (!checkCenter(i)) {
            // 1개 도시 선택
            for (int j = 0; j < 3; j++) {
                if (minWeight[j][i] <= limitDistance) {
                    key = center2key(1, { centers[j] });
                    recommendCities[key].push_back({ -prices[i], -minWeight[j][i], -i });
                    push_heap(recommendCities[key].begin(), recommendCities[key].end());
                }
            }

            // 3개 도시 선택
            key = center2key(3, centers);
            int w = minWeight[0][i] + minWeight[1][i] + minWeight[2][i];
            if (w <= limitDistance) {
                recommendCities[key].push_back({ -prices[i], -w, -i });
                push_heap(recommendCities[key].begin(), recommendCities[key].end());
            }

            // 2개 도시 선택
            for (auto s : select2) {
                key = center2key(2, { centers[s[0]], centers[s[1]] });
                w = minWeight[s[0]][i] + minWeight[s[1]][i];
                if (w <= limitDistance) {
                    recommendCities[key].push_back({ -prices[i], -w, -i });
                    push_heap(recommendCities[key].begin(), recommendCities[key].end());
                }
            }
        }
    }
}
int findCity(int mOpt, int mDestinations[])
{
    vector<int> destinations(mOpt);
    for (int i = 0; i < mOpt; i++) {
        destinations[i] = mDestinations[i];
    }
    int key = center2key(mOpt, destinations);

    if (recommendCities[key].empty()) { return -1; }

    vector<int> node;
    while (!recommendCities[key].empty()) {
        node = recommendCities[key].front();
        pop_heap(recommendCities[key].begin(), recommendCities[key].end());
        recommendCities[key].pop_back();

        if (-node[0] == prices[-node[2]]) {
            break;
        }
        else {
            recommendCities[key].push_back({ -prices[-node[2]], node[1], node[2] });
            push_heap(recommendCities[key].begin(), recommendCities[key].end());
        }
    }

    prices[-node[2]]++;
    recommendCities[key].push_back({ -prices[-node[2]], node[1], node[2] });
    push_heap(recommendCities[key].begin(), recommendCities[key].end());

    return -node[2];
}
