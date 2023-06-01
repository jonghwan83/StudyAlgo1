#define TABLESIZE 2048
#define MAXUSER 500
#define MAXARR 30000

class Node {
public:
    int id;
    int idx;
    int time;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;

    void init() {
        length = 0;
        head = nullptr;
    }

    void push(int id, int idx, int time) {
        Node* node = new Node();
        node->id = id; node->idx = idx; node->time = time;

        node->next = head;
        head = node;

        length++;
    }

    int find(int id) {
        Node* node = head;

        while (node) {
            if (node->id == id) { return node->idx; }
            node = node->next;
        }

        return -1;
    }
};

class Heap {
public:
    int length;
    Node arr[MAXARR];

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].time > arr[child].time) { return true; }
        if (arr[parent].time == arr[child].time && arr[parent].id < arr[child].id) { return true; }
        return false;
    }

    void push(int time, int id, int ix) {
        Node last{};
        last.time = time; last.id = id; last.idx = ix;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Node temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    Node pop() {
        Node ans = arr[0];
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
                Node temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};


class User {
public:
    int id;
    int resetTime;
    LinkedList channelIdx;

    void init() {
        resetTime = 0;
        channelIdx.init();
    }
};

class Channel {
public:
    int id;
    int nUser;
    Heap newsIdx;

    void init() {
        nUser = 0;
        newsIdx.init();
    }
};

class News {
public:
    int id;
    int channelIdx;
    bool isRemoved;

    void init() {
        isRemoved = false;
    }
};


LinkedList hashUser[TABLESIZE];
LinkedList hashNews[TABLESIZE];
LinkedList hashChannel[TABLESIZE];

int uIdx, cIdx, nIdx;

User users[MAXUSER];
Channel channels[MAXUSER];
News news[MAXARR];

Heap queueNews;


void init(int N, int K)
{
    queueNews.init();

    for (int i = 0; i < TABLESIZE; i++) {
        hashUser[i].init();
        hashNews[i].init();
        hashChannel[i].init();
    }

    uIdx = 0; cIdx = 0; nIdx = 0;
}

void updateTime(int mTime) {
    while (queueNews.arr[0].time <= mTime && queueNews.length > 0) {
        Node curr = queueNews.pop();

        if (news[curr.idx].isRemoved) { continue; }

        int channelIdx = news[curr.idx].channelIdx;

        channels[channelIdx].newsIdx.push(-curr.time, curr.id, curr.idx);
    }
}


void registerUser(int mTime, int mUID, int mNum, int mChannelIDs[])
{
    updateTime(mTime);

    int uKey = mUID % TABLESIZE;
    int userIdx = hashUser[uKey].find(mUID);

    if (userIdx == -1) {
        users[uIdx].init();

        hashUser[uKey].push(mUID, uIdx, mTime);

        users[uIdx].id = mUID;

        userIdx = uIdx;

        uIdx++;
    }

    for (int i = 0; i < mNum; i++) {
        int cKey = mChannelIDs[i] % TABLESIZE;
        int channelIdx = hashChannel[cKey].find(mChannelIDs[i]);

        if (channelIdx == -1) {
            channels[cIdx].init();

            hashChannel[cKey].push(mChannelIDs[i], cIdx, mTime);


            channels[cIdx].id = mChannelIDs[i];

            users[userIdx].channelIdx.push(mChannelIDs[i], cIdx, mTime);
            channels[cIdx].nUser++;

            cIdx++;
        }
        else {
            users[userIdx].channelIdx.push(mChannelIDs[i], channelIdx, mTime);
            channels[channelIdx].nUser++;
        }
    }
}



int offerNews(int mTime, int mNewsID, int mDelay, int mChannelID)
{
    updateTime(mTime);
    news[nIdx].init();

    queueNews.push(mTime + mDelay, mNewsID, nIdx);

    int cKey = mChannelID % TABLESIZE;
    int channelIdx = hashChannel[cKey].find(mChannelID);

    int nKey = mNewsID % TABLESIZE;

    hashNews[nKey].push(mNewsID, nIdx, mTime);
    news[nIdx].id = mNewsID;
    news[nIdx].channelIdx = channelIdx;

    nIdx++;
    return channels[channelIdx].nUser;
}



void cancelNews(int mTime, int mNewsID)

{
    updateTime(mTime);

    int nKey = mNewsID % TABLESIZE;
    int newsIdx = hashNews[nKey].find(mNewsID);

    news[newsIdx].isRemoved = true;
}



int checkUser(int mTime, int mUID, int mRetIDs[])

{
    updateTime(mTime);
    int ans = 0;

    Heap pQueue; pQueue.init();

    int uKey = mUID % TABLESIZE;
    int userIdx = hashUser[uKey].find(mUID);

    Node* node = users[userIdx].channelIdx.head;

    while (node) {

        for (int i = 0; i < channels[node->idx].newsIdx.length; i++) {
            Node curr = channels[node->idx].newsIdx.arr[i];
            if (news[curr.idx].isRemoved) { continue; }
            if (-curr.time <= users[userIdx].resetTime) { continue; }
            if (-curr.time <= node->time) { continue; }

            pQueue.push(curr.time, curr.id, curr.idx);
            ans++;
        }

        node = node->next;
    }

    for (int i = 0; i < 3; i++) {
        if (pQueue.length <= 0) { break; }
        mRetIDs[i] = pQueue.pop().id;
    }

    users[userIdx].resetTime = mTime;

    return ans;

}