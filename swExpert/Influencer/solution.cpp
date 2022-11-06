#include <vector>
#include <algorithm>
#include <deque>
#include <iostream>

#define MAXMEMBERS 20001

using namespace std;

int totalMembers;
vector< vector<int> > network(MAXMEMBERS);
vector<int> buyingPower(MAXMEMBERS);
vector< vector<int> > memberRanking;
vector<int> powers = { 10, 5, 3, 2 };
vector<int> hashPower(MAXMEMBERS);
vector<int> version(MAXMEMBERS);
vector<bool> checkDFS(MAXMEMBERS);

int calPower(int id, int depth) {
    deque< pair<int, int> > queue;
    vector<bool> visited(MAXMEMBERS);
    int result = 0;

    queue.push_back(make_pair(id, depth));
    visited[id] = true;
    result += (buyingPower[id] * powers[depth]);

    pair<int, int> cNode;
    while (!queue.empty()) {
        cNode = queue.front();
        queue.pop_front();
        for (auto next : network[cNode.first]) {
            if (cNode.second < 3 && !visited[next]) {
                visited[next] = true;
                result += (buyingPower[next] * powers[cNode.second + 1]);
                queue.push_back(make_pair(next, cNode.second + 1));
            }
        }
    }
    return result;
}

void init(int N, int mPurchasingPower[20000], int M, int mFriend1[20000], int mFriend2[20000])
{
    totalMembers = N;
    memberRanking.clear();

    for (int i = 0; i < M; i++) {
        network[i].clear();
    }

    for (int i = 0; i < M; i++) {
        network[mFriend1[i]].push_back(mFriend2[i]);
        network[mFriend2[i]].push_back(mFriend1[i]);
    }
    for (int i = 0; i < N; i++) {
        buyingPower[i] = mPurchasingPower[i];
        version[i] = 0;
    }

    int power;
    for (int i = 0; i < N; i++) {
        power = calPower(i, 0);
        hashPower[i] = power;
        memberRanking.push_back({ power, -i, 0 });
        push_heap(memberRanking.begin(), memberRanking.end());
    }
}

int influencer(int mRank)
{
    vector< vector<int> > temp;
    int k = 0;

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

void updatePower(int mID, int mPower, int depth) {
    deque< pair<int, int> > queue;
    vector<bool> visited(totalMembers);
    pair<int, int> cNode;

    queue.push_back(make_pair(mID, depth));
    visited[mID] = true;
    hashPower[mID] += (powers[depth] * mPower);
    memberRanking.push_back({ hashPower[mID], -mID, ++version[mID] });
    push_heap(memberRanking.begin(), memberRanking.end());

    while (!queue.empty()) {
        cNode = queue.front();
        queue.pop_front();

        for (int next : network[cNode.first]) {
            if (!visited[next] && cNode.second < 3) {
                visited[next] = true;
                hashPower[next] += (powers[cNode.second + 1] * mPower);
                memberRanking.push_back({ hashPower[next], -next, ++version[next] });
                push_heap(memberRanking.begin(), memberRanking.end());

                queue.push_back(make_pair(next, cNode.second + 1));
            }
        }
    }
}

int addPurchasingPower(int mID, int mPower)
{
    buyingPower[mID] += mPower;
    updatePower(mID, mPower, 0);

    return hashPower[mID];
}

void updateNet(int mID1, int mID2, int depth) {
    if (depth > 3) {
        return;
    }

    deque< pair<int, int> > queue;
    vector<bool> visited(totalMembers);

    queue.push_back(make_pair(mID2, depth));
    visited[mID2] = true;
    hashPower[mID1] += (buyingPower[mID2] * powers[depth]);

    pair<int, int> node;
    while (!queue.empty()) {
        node = queue.front();
        queue.pop_front();

        for (int node2 : network[node.first]) {
            if (!visited[node2] && node.second < 3) {
                visited[node2] = true;
                hashPower[mID1] += (buyingPower[node2] * powers[node.second + 1]);
                queue.push_back(make_pair(node2, node.second + 1));
            }
        }
    }

    memberRanking.push_back({ hashPower[mID1], -mID1, ++version[mID1] });
    push_heap(memberRanking.begin(), memberRanking.end());

    for (int next : network[mID1]) {
        if (!checkDFS[next]) {
            checkDFS[next] = true;
            updateNet(next, mID2, depth + 1);
        }
    }
}

int addFriendship(int mID1, int mID2)
{
    for (int i = 0; i < totalMembers; i++) {
        checkDFS[i] = false;
    }

    checkDFS[mID1] = true;
    updateNet(mID1, mID2, 1);
    checkDFS[mID2] = true;
    updateNet(mID2, mID1, 1);

    network[mID1].push_back(mID2);
    network[mID2].push_back(mID1);


    return hashPower[mID1] + hashPower[mID2];
}