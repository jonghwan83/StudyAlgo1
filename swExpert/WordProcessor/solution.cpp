#include <string.h>
#include <string>
#include <unordered_map>

#define MAXDICT 2000
#define MAXWORD 40000
#define MAXCHAR 11

using namespace std;

class Dictionary {
public:
	string word;
	bool isRemoved;

	Dictionary() {
		word.clear();
		isRemoved = false;
	}
};

int dictIdx[MAXCHAR];
int wIdx;
Dictionary dict[MAXCHAR][MAXDICT];
Dictionary words[MAXWORD];
unordered_map<string, int> hashDict;
unordered_map<int, int> sIdx;
unordered_map<int, int> eIdx;

void init(int N, char mString[]) {
	for (int i = 0; i < MAXCHAR; i++) {
		dictIdx[i] = 0;
	}
	wIdx = 0;
	hashDict.clear();
	sIdx.clear();
	eIdx.clear();

	string s = "";
	sIdx[0] = 0;
	for (int i = 0; i < N; i++) {
		if (mString[i] == '_') {
			words[wIdx].word = s;
			sIdx[i + 1] = wIdx + 1;
			eIdx[i - 1] = wIdx;
			wIdx++;
			s = "";
		}
		else if (i == N - 1) {
			s += mString[i];
			words[wIdx].word = s;
			eIdx[i] = wIdx;
			wIdx++;
		}
		else {
			s += mString[i];
		}
	}

}

void addWord(char mWord[])
{
	unordered_map<string, int>::iterator it = hashDict.find(mWord);
	if (it == hashDict.end()) {
		int key = strlen(mWord);
		dict[key][dictIdx[key]].word = mWord;
		dict[key][dictIdx[key]].isRemoved = false;
		hashDict[mWord] = dictIdx[key];
		dictIdx[key]++;
	}
	else {
		int idx = hashDict[mWord];
		int key = strlen(mWord);
		dict[key][idx].isRemoved = false;
	}
}

void removeWord(char mWord[])
{
	int idx = hashDict[mWord];
	int key = strlen(mWord);
	dict[key][idx].isRemoved = true;
}

int checkDiff(string word1, string word2) {
	if (word1.size() != word2.size()) {
		return 12;
	}
	
	int ans = 0;

	for (int i = 0; i < word1.size(); i++) {
		if (word1[i] != word2[i]) {
			ans++;
		}
	}

	return ans;
}

string modifying(int i) {
	string s = "";

	int key = words[i].word.size();

	for (int j = 0; j < dictIdx[key]; j++) {
		if (dict[key][j].isRemoved) { continue; }
		int diff = checkDiff(words[i].word, dict[key][j].word);
		if (diff == 12) { continue; }
		
		if (s.size() > 0) {
			if (diff == 1 && s > dict[key][j].word) {
				s = dict[key][j].word;
			}
		}
		else {
			if (diff == 1) {
				s = dict[key][j].word;
			}
		}
	}
	return s;
}


int correct(int mStart, int mEnd)
{
	int st = sIdx[mStart];
	int ed = eIdx[mEnd];

	int ans = 0;
	int diff;

	for (int i = st; i <= ed; i++) {

		unordered_map<string, int>::iterator it = hashDict.find(words[i].word);
		if (it == hashDict.end()) {
			string s = modifying(i);
			if (s.size() > 0) {
				words[i].word = s;
				ans++;
			}
		}
		else {
			int idx = hashDict[words[i].word];
			int key = words[i].word.size();
			if (dict[key][idx].isRemoved) {
				string s = modifying(i);
				if (s.size() > 0) {
					words[i].word = s;
					ans++;
				}
			}
			else { continue; }
		}
	}
	return ans;

}

void destroy(char mResult[])
{
	int k = 0;
	for (int i = 0; i < wIdx; i++) {
		for (int j = 0; j < words[i].word.size(); j++) {
			mResult[k] = words[i].word[j];
			k++;
		}
		mResult[k] = '_';
		k++;
	}
	mResult[k] = '\0';
}