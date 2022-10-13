#include <vector>
#include <unordered_map>
#include <deque>
#include <iostream>

#define VMAX 1e9;

using namespace std;


unordered_map<int, vector< vector<int> > > graphForward;  // key: start, value: destination, weight
unordered_map<int, vector< vector<int> > > graphBackward;

int getMinCost(int start, bool direction) {
    unordered_map<int, int> minCost;
    for (auto it = graphForward.begin(); it != graphForward.end(); it++) {
        minCost[it->first] = VMAX;
    }

    deque<pair<int, int> > queue;

    queue.push_back(make_pair(start, 0));
    minCost[start] = 0;

    int cCity, w;
    while (queue.size() > 0) {
        cCity = queue.front().first;
        w = queue.front().second;
        queue.pop_front();

        if (minCost[cCity] < w) { continue; }

        if (direction) {
            for (int i = 0; i < graphForward[cCity].size(); i++) {
                if (minCost[graphForward[cCity][i][0]] > (minCost[cCity] + graphForward[cCity][i][1])) {
                    minCost[graphForward[cCity][i][0]] = minCost[cCity] + graphForward[cCity][i][1];
                    queue.push_back(make_pair(graphForward[cCity][i][0], graphForward[cCity][i][1]));
                }
            }
        }
        else {
            for (int i = 0; i < graphBackward[cCity].size(); i++) {
                if (minCost[graphBackward[cCity][i][0]] > (minCost[cCity] + graphBackward[cCity][i][1])) {
                    minCost[graphBackward[cCity][i][0]] = minCost[cCity] + graphBackward[cCity][i][1];
                    queue.push_back(make_pair(graphBackward[cCity][i][0], graphBackward[cCity][i][1]));
                }
            }
        }
    }

    int ans = 0;
    for (auto it = minCost.begin(); it != minCost.end(); it++) {
        ans += minCost[it->first];
    }

    return ans;
}

int init(int N, int sCity[], int eCity[], int mCost[]) {
    graphForward.clear();
    graphBackward.clear();

    for (int i = 0; i < N; i++) {
        graphForward[sCity[i]].push_back({ eCity[i], mCost[i] });
        graphBackward[eCity[i]].push_back({ sCity[i], mCost[i] });
    }
    return (int)graphForward.size();
}

void add(int sCity, int eCity, int mCost) {
    graphForward[sCity].push_back({ eCity, mCost });
    graphBackward[eCity].push_back({ sCity, mCost });

    return;
}

int cost(int mHub) {
    int ans = 0;

    ans += getMinCost(mHub, 1);
    ans += getMinCost(mHub, 0);

    return ans;
}
