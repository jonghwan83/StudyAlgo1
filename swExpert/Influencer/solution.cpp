#include <vector>
#include <algorithm>
#include <deque>

#define MAXMEMBERS 20000

using namespace std;

int totalmember;
vector<int> depthPower = { 10, 5, 3, 2 };
vector< vector < vector< pair<int, bool> > > > graph(MAXMEMBERS, vector< vector< pair<int, bool> > >(4));
vector<int> buyingPower(MAXMEMBERS);
vector<int> hashInfluence(MAXMEMBERS);
vector< vector<int> > memberRanking;
vector<int> version(MAXMEMBERS);
vector<bool> checkDFS(MAXMEMBERS);

void updateGraph(int start) {
    deque< pair<int, int> > queue;
    vector<bool> visited(MAXMEMBERS);
    visited[start] = true;

    for (int i = 0; i < graph[start][1].size(); i++) {
        queue.push_back(make_pair(graph[start][1][i].first, 1));
        visited[graph[start][1][i].first] = true;
        hashInfluence[start] += (buyingPower[graph[start][1][i].first] * depthPower[1]);
    }

    pair<int, int> node;
    while (!queue.empty()) {
        node = queue.front();
        queue.pop_front();

        if (node.second < 3) {
            for (int i = 0; i < graph[node.first][1].size(); i++) {
                if (!visited[graph[node.first][1][i].first]) {
                    graph[start][node.second + 1].push_back(make_pair(graph[node.first][1][i].first, true));
                    visited[graph[node.first][1][i].first] = true;
                    hashInfluence[start] += (buyingPower[graph[node.first][1][i].first] * depthPower[node.second + 1]);
                    queue.push_back(make_pair(graph[node.first][1][i].first, node.second + 1));
                }
            }
        }
    }
}

void init(int N, int mPurchasingPower[20000], int M, int mFriend1[20000], int mFriend2[20000])
{
    memberRanking.clear();
    totalmember = N;
    for (int i = 0; i < N; i++) {
        version[i] = 0;
        buyingPower[i] = mPurchasingPower[i];
        for (int j = 0; j < 4; j++) {
            graph[i][j].clear();
        }
        graph[i][0].push_back(make_pair(i, true));
        hashInfluence[i] = (buyingPower[i] * depthPower[0]);
    }

    for (int i = 0; i < M; i++) {
        graph[mFriend1[i]][1].push_back(make_pair(mFriend2[i], true));
        graph[mFriend2[i]][1].push_back(make_pair(mFriend1[i], true));
    }

    for (int i = 0; i < totalmember; i++) {
        updateGraph(i);
        memberRanking.push_back({ hashInfluence[i], -i, 0 });
        push_heap(memberRanking.begin(), memberRanking.end());
    }
}

int influencer(int mRank)
{
    int k = 0;
    vector< vector<int> > temp;
    while (k < mRank) {
        if (memberRanking.front()[2] == version[-memberRanking.front()[1]]) {
            temp.push_back(memberRanking.front());
            pop_heap(memberRanking.begin(), memberRanking.end());
            memberRanking.pop_back();
            k++;
        }
        else {
            pop_heap(memberRanking.begin(), memberRanking.end());
            memberRanking.pop_back();
        }
    }

    int ans = -temp.back()[1];

    for (auto t : temp) {
        memberRanking.push_back(t);
        push_heap(memberRanking.begin(), memberRanking.end());
    }

    return ans;
}

void updatePower(int start, int power) {
    for (int i = 0; i < 4; i++) {
        for (auto node : graph[start][i]) {
            if (node.second) {
                hashInfluence[node.first] += (power * depthPower[i]);
                memberRanking.push_back({ hashInfluence[node.first], -node.first, ++version[node.first] });
                push_heap(memberRanking.begin(), memberRanking.end());
            }
        }
    }
}

int addPurchasingPower(int mID, int mPower)
{
    updatePower(mID, mPower);
    buyingPower[mID] += mPower;

    return hashInfluence[mID];
}

void addNode(int member1, int member2, int depth, int relativeDepth) {
    if (relativeDepth > 3) { return; }

    for (auto m2 : graph[member2][depth]) {
        if (m2.second && member1 != m2.first) {
            bool connectFlag = true;
            for (int i = 1; i <= relativeDepth; i++) {
                for (int j = 0; j < graph[member1][i].size(); j++) {
                    if (graph[member1][i][j].second && graph[member1][i][j].first == m2.first) {
                        connectFlag = false;
                    }
                }
            }

            if (connectFlag) {
                hashInfluence[member1] += (buyingPower[m2.first] * depthPower[relativeDepth]);
                for (int i = relativeDepth + 1; i < 4; i++) {
                    for (int j = 0; j < graph[member1][i].size(); j++) {
                        if (graph[member1][i][j].second && graph[member1][i][j].first == m2.first) {
                            graph[member1][i][j].second = false;
                            hashInfluence[member1] -= (buyingPower[m2.first] * depthPower[i]);
                        }
                    }
                }
            }
        }
    }
    addNode(member1, member2, depth + 1, relativeDepth + 1);
}

void reconnectNode(int member1, int member2, int depth, int relativeDepth) {
    if (relativeDepth > 3) { return; }
    
    for (auto m2 : graph[member2][depth]) {
        if (m2.second && member1 != m2.first) {
            bool connectFlag = true;
            for (int i = 1; i <= relativeDepth; i++) {
                for (int j = 0; j < graph[member1][i].size(); j++) {
                    if (graph[member1][i][j].second && graph[member1][i][j].first == m2.first) {
                        connectFlag = false;
                    }
                }
            }
            if (connectFlag) {
                graph[member1][relativeDepth].push_back(make_pair(m2.first, true));

                for (int i = relativeDepth + 1; i < 4; i++) {
                    for (int j = 0; j < graph[member1][i].size(); j++) {
                        if (graph[member1][i][j].second && graph[member1][i][j].first == m2.first) {
                            graph[member1][i][j].second = false;
                        }
                    }
                }
            }
        }
    }
    reconnectNode(member1, member2, depth + 1, relativeDepth + 1);
}

int addFriendship(int mID1, int mID2)
{
    for (int depth = 0; depth < 3; depth++) {
        for (auto m1 : graph[mID1][depth]) {
            if (m1.second) {
                addNode(m1.first, mID2, 0, depth + 1);
                memberRanking.push_back({ hashInfluence[m1.first], -m1.first, ++version[m1.first] });
                push_heap(memberRanking.begin(), memberRanking.end());
            }
        }
        for (auto m2 : graph[mID2][depth]) {
            if (m2.second) {
                addNode(m2.first, mID1, 0, depth + 1);
                memberRanking.push_back({ hashInfluence[m2.first], -m2.first, ++version[m2.first] });
                push_heap(memberRanking.begin(), memberRanking.end());
            }
        }
    }

    for (int depth = 0; depth < 3; depth++) {
        for (auto m1 : graph[mID1][depth]) {
            if (m1.second) {
                reconnectNode(m1.first, mID2, 0, depth + 1);
            }
        }
        for (auto m2 : graph[mID2][depth]) {
            if (m2.second) {
                reconnectNode(m2.first, mID1, 0, depth + 1);
            }
        }
    }
    return hashInfluence[mID1] + hashInfluence[mID2];
}