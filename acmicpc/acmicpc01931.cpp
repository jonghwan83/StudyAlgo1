#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(){
    int N;
    scanf("%d", &N);
    vector<vector < long long int> > meetings;

    long long int a, b;
    for (int i=0; i < N; i++){
        scanf("%lld %lld", &a, &b);
        vector<long long int> temp;
        temp.push_back(a);
        temp.push_back(b);
        meetings.push_back(temp);
    }
    sort(begin(meetings), end(meetings));

    long long int intervals[N];
    for (int i=0; i < N; i++){
        intervals[i] = meetings[i][1] - meetings[i][0];
    }

    int cum[N];
    cum[0] = 1;
    long long int nTime[N];
    nTime[0] = meetings[0][1];

    for (int i=1; i < N; i++){
        cum[i] = cum[i-1];
        nTime[i] = nTime[i-1];
        if (meetings[i][0] >= nTime[i]){
            cum[i] = cum[i] + 1;
            nTime[i] = meetings[i][1];
        } else if (intervals[i] < intervals[i-1]){
            nTime[i] = min(meetings[i][1], nTime[i]);
        }
    }
    printf("%d\n", cum[N-1]);
}