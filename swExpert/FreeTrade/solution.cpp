#include <string>
#include <unordered_map>

#define MAXN   (500)
#define MAXL   (11)

#define MAXCOMPANY 1000

using namespace std;

class Company {
public:
	string name;
	int gIdx;

	Company() {
		name = "";
		gIdx = -1;
	}
};

class Node {
public:
	int cIdx;
	Node* next;
	
	Node() {
		cIdx = -1;
		next = NULL;
	}
};

class LinkedList {
public:
	Node* head;
	Node* tail;
	int length;
	int partners[MAXCOMPANY];

	LinkedList() {
		head = NULL;
		tail = NULL;
		length = 0;
		for (int i = 0; i < MAXCOMPANY; i++) {
			partners[i] = 0;
		}
	}

	void clear() {
		head = NULL;
		tail = NULL;
		length = 0;
		for (int i = 0; i < MAXCOMPANY; i++) {
			partners[i] = 0;
		}
	}

	void addNode(int a) {
		Node* newNode = new Node();
		newNode->cIdx = a;

		newNode->next = head;
		if (!tail) { tail = newNode; }

		head = newNode;
		length++;
	}
};

int companyIdx, totalCompany, allyIdx;
Company companies[MAXCOMPANY];
LinkedList alliance[MAXCOMPANY];
unordered_map<string, int> hashCompany;


void assembleLink(int cIdx1, int cIdx2) {
	int aIdx1 = companies[cIdx1].gIdx;
	int aIdx2 = companies[cIdx2].gIdx;

	if (aIdx1 == aIdx2) { return; }

	Node* cNode = alliance[aIdx2].head;
	while (cNode) {
		companies[cNode->cIdx].gIdx = aIdx1;
		cNode = cNode->next;
	}

	for (int i = 0; i < totalCompany; i++) {
		if (alliance[aIdx2].partners[i]) {
			alliance[aIdx1].partners[i] += alliance[aIdx2].partners[i];
			alliance[i].partners[aIdx1] += alliance[aIdx2].partners[i];
		}
	}

	alliance[aIdx1].tail->next = alliance[aIdx2].head;
	alliance[aIdx1].tail = alliance[aIdx2].tail;
	alliance[aIdx1].length += alliance[aIdx2].length;

	alliance[aIdx2].clear();
}

bool checkIdx(int cIdx, int gIdx) {
	Node* cNode = alliance[gIdx].head;
	while (cNode) {
		if (cNode->cIdx == cIdx) { return true; }
		cNode = cNode->next;
	}

	return false;
}

void addCompany(char mCompany[MAXL]) {
	companies[companyIdx].name = mCompany;
	companies[companyIdx].gIdx = companyIdx;
	alliance[companyIdx].clear();
	alliance[companyIdx].addNode(companyIdx);
	hashCompany[mCompany] = companyIdx;
	companyIdx++;
}

void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL])
{
	companyIdx = 0;
	hashCompany.clear();
	totalCompany = mNumA + mNumB;
	for (int i = 0; i < mNumA; i++) {
		addCompany(mCompanyListA[i]);
	}

	for (int i = 0; i < mNumB; i++) {
		addCompany(mCompanyListB[i]);
	}
}

void startProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
	int cIdx1 = hashCompany[mCompanyA];
	int cIdx2 = hashCompany[mCompanyB];

	int gIdx1 = companies[cIdx1].gIdx;
	int gIdx2 = companies[cIdx2].gIdx;

	alliance[gIdx1].partners[gIdx2]++;
	alliance[gIdx2].partners[gIdx1]++;
}

void finishProject(char mCompanyA[MAXL], char mCompanyB[MAXL])
{
	int cIdx1 = hashCompany[mCompanyA];
	int cIdx2 = hashCompany[mCompanyB];

	int gIdx1 = companies[cIdx1].gIdx;
	int gIdx2 = companies[cIdx2].gIdx;

	alliance[gIdx1].partners[gIdx2]--;
	alliance[gIdx2].partners[gIdx1]--;

}

void ally(char mCompany1[MAXL], char mCompany2[MAXL])
{
	int cIdx1 = hashCompany[mCompany1];
	int cIdx2 = hashCompany[mCompany2];

	int aIdx1 = companies[cIdx1].gIdx;
	int aIdx2 = companies[cIdx2].gIdx;

	if (alliance[aIdx1].length > alliance[aIdx2].length) {
		assembleLink(cIdx1, cIdx2);
	}
	else {
		assembleLink(cIdx2, cIdx1);
	}
}


int conflict(char mCompany1[MAXL], char mCompany2[MAXL])
{
	int cIdx1 = hashCompany[mCompany1];
	int cIdx2 = hashCompany[mCompany2];

	int gIdx1 = companies[cIdx1].gIdx;
	int gIdx2 = companies[cIdx2].gIdx;

	int ans = 0;
	for (int i = 0; i < totalCompany; i++) {
		if (alliance[gIdx1].partners[i] > 0 && alliance[gIdx2].partners[i] > 0) {
			ans += alliance[i].length;
		}
	}

	return ans;
}
