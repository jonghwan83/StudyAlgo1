#define MAX_N 50000
#define MAX_M 50000
#define MAX_LEN 11
#define ALPHABET 26

#include <string.h>
#include <string>
#include <unordered_map>


class HeapNode {
public:
    char data[MAX_LEN];
};

class Heap {
public:
    int length;
    HeapNode arr[MAX_M];

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (strcmp(arr[parent].data, arr[child].data) > 0) { return true; }
        return false;
    }

    void push(char s[MAX_LEN]) {
        HeapNode last;
        strcpy(last.data, s);

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (idx * 2 + 1 < length) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};

class Player {
public:
    bool isRemoved;
};

std::unordered_map<std::string, bool> hashDict;
Heap dictionary[ALPHABET];
Player players[MAX_N + 1];
int totalPlayer;

int currentPlayer(int a) {

    while (players[a].isRemoved || a > totalPlayer) {
        if (a > totalPlayer) {
            a = 1;
            continue;
        }
        a++;
    }

    return a;
}

void reverse(char s[MAX_LEN]) {
    int k = strlen(s);
    for (int i = 0; i < k / 2; i++) {
        char temp = s[i];
        s[i] = s[k - 1 - i];
        s[k - 1 - i] = temp;
    }
}

void init(int N, int M, char mWords[][MAX_LEN])
{
    totalPlayer = N;
    hashDict.clear();

    for (int i = 1; i < N + 1; i++) {
        players[i].isRemoved = false;
    }

    for (int i = 0; i < ALPHABET; i++) {
        dictionary[i].init();
    }

    for (int i = 0; i < M; i++) {
        int key = (int) mWords[i][0] - 'a';
        dictionary[key].push(mWords[i]);
    }
}

int playRound(int mID, char mCh)
{
    int player = mID;
    int key = mCh - 'a';

    HeapNode temp[MAX_M];
    int tIdx = 0;

    while (dictionary[key].length > 0) {
        HeapNode curr = dictionary[key].pop();

        std::unordered_map<std::string, bool>::iterator itr = hashDict.find(curr.data);
        if (itr != hashDict.end()) { continue; }
        hashDict[curr.data] = true;

        int k = strlen(curr.data);
        key = (int) curr.data[k - 1] - 'a';
        player = currentPlayer(player + 1);

        reverse(curr.data);
        temp[tIdx++] = curr;
    }

    for (int i = 0; i < tIdx; i++) {
        std::unordered_map<std::string, bool>::iterator itr = hashDict.find(temp[i].data);
        if (itr == hashDict.end()) {
            key = (int) temp[i].data[0] - 'a';
            dictionary[key].push(temp[i].data);
        }
    }

    players[player].isRemoved = true;

    return player;
}