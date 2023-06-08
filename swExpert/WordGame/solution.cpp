#define MAXL 10
#define MAXCARD 10000
#define MAXPLAYER 50
#define ALPHABET 26
#define MAXDECK 10000

#include <cstring>

using namespace std;

class CardDeck {
public:
    char word[MAXL + 1];
    char subject[MAXL + 1];
    int players[MAXPLAYER + 1];
    int cnt;

    void init() {
        cnt = 0;
        for (int i = 0; i < MAXPLAYER + 1; i++) {
            players[i] = 0;
        }
    }
};

CardDeck deck[MAXDECK + 1];


class Card {
public:
    int number;
};

class Heap {
public:
    int length;
    Card arr[MAXCARD + 1];

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (deck[arr[parent].number].cnt < deck[arr[child].number].cnt) { return true; }
        if (deck[arr[parent].number].cnt == deck[arr[child].number].cnt && strcmp(deck[arr[parent].number].word, deck[arr[child].number].word) > 0) { return true; }
        return false;
    }

    void push(int card) {
        int idx = length;
        arr[length].number = card;;
        length++;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Card temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }
};



class Player {
public:
    Card cards[ALPHABET][ALPHABET][MAXCARD + 1];
    int length[ALPHABET][ALPHABET];
    int score;
    bool isRemoved;

    void init() {
        isRemoved = false;
        score = 0;
        for (int i = 0; i < ALPHABET; i++) {
            for (int j = 0; j < ALPHABET; j++) {
                length[i][j] = 0;
            }
        }
    }

    void push(int cardIdx) {
        int key1 = deck[cardIdx].word[0] - 'a';
        int key2 = deck[cardIdx].word[1] - 'a';

        cards[key1][key2][length[key1][key2]].number = cardIdx;
        length[key1][key2]++;
    }
};

Player players[MAXPLAYER + 1];
int totalPlayer;
int totalCard;

void init(int N, char mWordList[][MAXL + 1], char mSubjectList[][MAXL + 1])
{   
    totalPlayer = 0;
    totalCard = N;

    for (int i = 0; i < N; i++) {
        deck[i + 1].init();
        strcpy(deck[i + 1].word, mWordList[i]);
        strcpy(deck[i + 1].subject, mSubjectList[i]);

    }

    return;
}


void join(int mID, int M, int mCardList[])
{
    players[mID].init();

    for (int i = 0; i < M; i++) {
        int key1 = deck[mCardList[i]].word[0] - 'a';
        int key2 = deck[mCardList[i]].word[1] - 'a';

        if (deck[mCardList[i]].players[mID] == 0) {
            deck[mCardList[i]].cnt++;
        }
        deck[mCardList[i]].players[mID]++;
        
        players[mID].push(mCardList[i]);
    }

    totalPlayer++;

    return;
}

int getCard(int mID, char mBeginStr[], char mSubject[]) {
    int key1 = mBeginStr[0] - 'a';
    int key2 = -1;
    if (strlen(mBeginStr) > 1) {
        key2 = mBeginStr[1] - 'a';
    }
    
    Heap pQueue;
    pQueue.init();

    if (key2 == -1) {
        for (int i = 0; i < ALPHABET; i++) {
            for (int k = 0; k < players[mID].length[key1][i]; k++) {
                int card = players[mID].cards[key1][i][k].number;
                if (deck[card].players[mID] > 0 && strcmp(deck[card].subject, mSubject) == 0 && deck[card].word[0] == mBeginStr[0]) {
                    pQueue.push(card);
                }
            }
        }
    }
    else {
        for (int k = 0; k < players[mID].length[key1][key2]; k++) {
            int card = players[mID].cards[key1][key2][k].number;
            if (deck[card].players[mID] > 0 && strcmp(deck[card].subject, mSubject) == 0 && deck[card].word[0] == mBeginStr[0] && deck[card].word[1] == mBeginStr[1]) {
                pQueue.push(card);
            }
        }
    }

    if (pQueue.length > 0) {
        return pQueue.arr[0].number;   
    }

    return 0;
}

int playRound(char mBeginStr[], char mSubject[])
{
    int ans = 0;
    int players_card[MAXPLAYER + 1];
    int cardlist[MAXPLAYER + 1];
    int cnt[MAXCARD + 1];
    int tIdx = 0;

    for (int i = 1; i <= totalCard; i++) {
        cnt[i] = 0;
    }

    for (int i = 1; i <= totalPlayer; i++) {
        players_card[i] = 0;
        cardlist[i] = 0;
    }

    for (int i = 1; i <= totalPlayer; i++) {
        if (players[i].isRemoved) { continue; }

        int card = getCard(i, mBeginStr, mSubject);

        ans += card;

        if (card > 0) {
            players_card[tIdx] = i;
            cardlist[tIdx] = card;
            cnt[card]++;
            tIdx++;
        }
    }

    for (int i = 0; i < tIdx; i++) {
        players[players_card[i]].score += (cnt[cardlist[i]] - 1) * (cnt[cardlist[i]] - 1);

        deck[cardlist[i]].players[players_card[i]]--;

        if (deck[cardlist[i]].players[players_card[i]] == 0) {
            deck[cardlist[i]].cnt--;
        }
    }

    return ans;
}



int leave(int mID)
{

    players[mID].isRemoved = true;

    for (int i = 0; i < ALPHABET; i++) {
        for (int j = 0; j < ALPHABET; j++) {

            for (int k = 0; k < players[mID].length[i][j]; k++) {
                int card = players[mID].cards[i][j][k].number;

                if (deck[card].players[mID] > 0) {
                    deck[card].players[mID]--;

                    if (deck[card].players[mID] == 0) {
                        deck[card].cnt--;
                    }
                }
                
            }
        }
    }

    return players[mID].score;

}
