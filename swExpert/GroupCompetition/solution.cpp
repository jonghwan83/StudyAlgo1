#define MAXPLAYER 100001

using namespace std;

class Player {
public:
    int id;
    int score;
    int gIdx;

    Player() {
        id = 0;
        score = 0;
        gIdx = -1;
    }
};

class Node {
public:
    int pIdx;
    Node* next;

    Node() {
        pIdx = -1;
        next = nullptr;
    }
};

class LinkedList {
public:
    Node* head;
    Node* tail;
    int length;
    int score;

    LinkedList() {
        head = nullptr;
        tail = nullptr;
        score = 0;
        length = 0;
    }
    void addNode(int a) {
        Node* newNode = new Node();
        newNode->pIdx = a;

        if (!tail) { tail = newNode; }

        newNode->next = head;
        head = newNode;
        length++;
    }

    void clear() {
        head = nullptr;
        tail = nullptr;
        score = 0;
        length = 0;
    }
};


int totalPlayers, groupLength;
Player players[MAXPLAYER];
LinkedList groups[MAXPLAYER];

void assembleLink(int mPlayerA, int mPlayerB) {
    int gIdx1 = players[mPlayerA].gIdx;
    int gIdx2 = players[mPlayerB].gIdx;

    Node* cNode = groups[gIdx2].head;
    while (cNode) {
        players[cNode->pIdx].score += (groups[gIdx2].score - groups[gIdx1].score);
        players[cNode->pIdx].gIdx = gIdx1;

        cNode = cNode->next;
    }

    groups[gIdx1].tail->next = groups[gIdx2].head;
    groups[gIdx1].tail = groups[gIdx2].tail;
    groups[gIdx1].length += groups[gIdx2].length;

    groups[gIdx2].clear();
}

void init(int N)
{
    totalPlayers = N + 1;

    groupLength = 0;
    for (int i = 1; i < totalPlayers; i++) {
        players[i].score = 0;
        players[i].id = i;
        players[i].gIdx = -1;

        groups[i - 1].clear();
    }
}

void updateScore(int mWinnerID, int mLoserID, int mScore)
{
    if (players[mWinnerID].gIdx == -1) {
        players[mWinnerID].score += mScore;
    }
    else {
        groups[players[mWinnerID].gIdx].score += mScore;
    }

    if (players[mLoserID].gIdx == -1) {
        players[mLoserID].score -= mScore;
    }
    else {
        groups[players[mLoserID].gIdx].score -= mScore;
    }
}

void unionTeam(int mPlayerA, int mPlayerB)
{
    if (players[mPlayerA].gIdx == -1 && players[mPlayerB].gIdx == -1) {
        groups[groupLength].addNode(mPlayerA);
        groups[groupLength].addNode(mPlayerB);

        players[mPlayerA].gIdx = groupLength;
        players[mPlayerB].gIdx = groupLength;

        groupLength++;
    }
    else if (players[mPlayerB].gIdx == -1) {
        groups[players[mPlayerA].gIdx].addNode(mPlayerB);
        players[mPlayerB].score -= groups[players[mPlayerA].gIdx].score;

        players[mPlayerB].gIdx = players[mPlayerA].gIdx;
    }
    else if (players[mPlayerA].gIdx == -1) {
        groups[players[mPlayerB].gIdx].addNode(mPlayerA);
        players[mPlayerA].score -= groups[players[mPlayerB].gIdx].score;

        players[mPlayerA].gIdx = players[mPlayerB].gIdx;
    }
    else {
        if (groups[players[mPlayerA].gIdx].length >= groups[players[mPlayerB].gIdx].length) {
            assembleLink(mPlayerA, mPlayerB);
        }
        else {
            assembleLink(mPlayerB, mPlayerA);
        }
    }
}

int getScore(int mID)
{
    if (players[mID].gIdx == -1) { return players[mID].score; }

    return players[mID].score + groups[players[mID].gIdx].score;
}
