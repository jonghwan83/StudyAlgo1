#include <string.h>
#include <string>
#include <unordered_map>

#define ALPHABET 26
#define MAXWORD 1000
#define MAXCHAR 11

class HeapNode {
public:
    char data[MAXCHAR];
    bool isRemoved;

    void init() {
        isRemoved = false;
    }
};

HeapNode charArr[MAXWORD];
int cIdx;

class Heap {
public:
    int arr[MAXWORD];
    int length;

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (strcmp(charArr[arr[parent]].data, charArr[arr[child]].data) > 0) { return true; }
        return false;
    }

    void push(int a) {
        int idx = length;
        arr[length++] = a;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            int temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    int pop() {
        int ans = arr[0];
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
                int temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};

class Word {
public:
    char data[MAXCHAR];
};

Heap dictionary[ALPHABET][ALPHABET];
Word words[20000];
int wIdx;

char str1[MAXCHAR];
char str2[MAXCHAR];

std::unordered_map<std::string, int> hashDict;
std::unordered_map<int, int> hashStart;
std::unordered_map<int, int> hashEnd;

void init(int N, char mString[])
{
    hashDict.clear();
    hashStart.clear();
    hashEnd.clear();

    cIdx = 0;
    for (int i = 0; i < ALPHABET; i++) {
        for (int j = 0; j < ALPHABET; j++) {
            dictionary[i][j].init();
        }
    }

    wIdx = 0;
    char temp[MAXCHAR];
    int k = 0;
    hashStart[0] = 0;

    for (int i = 0; i < N; i++) {
        if (mString[i] == '_') {
            temp[k] = '\0';
            hashEnd[i - 1] = wIdx;
            strcpy(words[wIdx++].data, temp);

            hashStart[i + 1] = wIdx;
            k = 0;
            continue;
        }
        temp[k] = mString[i];
        k++;

        if (i == N - 1) {
            temp[k] = '\0';
            hashEnd[i] = wIdx;
            strcpy(words[wIdx++].data, temp);
        }
    }
}

void addWord(char mWord[])
{
    int key1 = (int) mWord[0] - 'a';
    int key2 = (int) mWord[1] - 'a';

    charArr[cIdx].init();
    strcpy(charArr[cIdx].data, mWord);
    dictionary[key1][key2].push(cIdx);
    hashDict[mWord] = cIdx;

    cIdx++;
}

void removeWord(char mWord[])
{
    std::unordered_map<std::string, int>::iterator itr = hashDict.find(mWord);
    charArr[itr->second].isRemoved = true;
}

bool search(int key, int loc, int idx) {

    bool isExist = false;

    for (int i = 0; i < ALPHABET; i++) {
        int temp[MAXWORD];
        int tIdx = 0;

        if (loc == 0) {
            while (dictionary[key][i].length > 0 && !isExist) {
                int curr = dictionary[key][i].pop();
                if (charArr[curr].isRemoved) { continue; }

                temp[tIdx++] = curr;
                if (strlen(words[idx].data) != strlen(charArr[curr].data)) { continue; }

                int k = 0;
                for (int i = 0; i < strlen(words[idx].data); i++) {
                    if (words[idx].data[i] != charArr[curr].data[i]) { k++; }
                }

                if (k == 1) {
                    strcpy(str1, charArr[curr].data);
                    isExist = true;
                }
            }

            for (int t = 0; t < tIdx; t++) {
                dictionary[key][i].push(temp[t]);
            }
        }
		
        else {
            while (dictionary[i][key].length > 0 && !isExist) {
                int curr = dictionary[i][key].pop();
                if (charArr[curr].isRemoved) { continue; }

                temp[tIdx++] = curr;
                if (strlen(words[idx].data) != strlen(charArr[curr].data)) { continue; }

                int k = 0;
                for (int i = 0; i < strlen(words[idx].data); i++) {
                    if (words[idx].data[i] != charArr[curr].data[i]) { k++; }
                }

                if (k == 1) {
                    strcpy(str2, charArr[curr].data);
                    isExist = true;
                }
            }

            for (int t = 0; t < tIdx; t++) {
                dictionary[i][key].push(temp[t]);
            }
        }
    }

    return isExist;
}

bool isModified(int idx) {
    int temp[MAXWORD];
    int tIdx = 0;

    int key1 = (int) words[idx].data[0] - 'a';
    int key2 = (int) words[idx].data[1] - 'a';

    str1[0] = '~'; str2[0] = '~';

    bool isExist1 = search(key1, 0, idx);
    bool isExist2 = search(key2, 1, idx);

    if (isExist1 || isExist2) {
        if (strcmp(str1, str2) < 0) {
            strcpy(words[idx].data, str1);
        }
        else {
            strcpy(words[idx].data, str2);
        }
    }

    return isExist1 || isExist2;
}

int correct(int mStart, int mEnd)
{
    int ans = 0;

    std::unordered_map<int, int>::iterator itr_st = hashStart.find(mStart);
    std::unordered_map<int, int>::iterator itr_ed = hashEnd.find(mEnd);

    for (int i = itr_st->second; i <= itr_ed->second; i++) {
        std::unordered_map<std::string, int>::iterator itr_str = hashDict.find(words[i].data);
        if (itr_str == hashDict.end()) {
            if (isModified(i)) { ans++; }
        }
        else if (charArr[itr_str->second].isRemoved) {
            if (isModified(i)) { ans++; }
        }
    }

    return ans;
}

void destroy(char mResult[])
{
    int k = 0;
    for (int i = 0; i < wIdx; i++) {
        for (int j = 0; j < strlen(words[i].data); j++) {
            mResult[k] = words[i].data[j];
            k++;
        }
        mResult[k] = '_';
        k++;
    }
    mResult[k] = '\0';
}