#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int n, m, answer;
int arr[50][50];
vector< pair<int, int> > chickens, houses;
vector< vector<int> > distances;
vector<int> check;

void input(){
    answer = 99999;
    cin >> n >> m;

    for (int i=0; i < n; i++){
        for (int j=0; j < n ; j++){
            cin >> arr[i][j];
            if (arr[i][j] == 1){
                houses.push_back(make_pair(i, j));
            } else if (arr[i][j] == 2){
                chickens.push_back(make_pair(i, j));
                check.push_back(0);
            }
        }
    }
}

void getPosition(){
    for (int i=0; i < houses.size(); i++){
        vector<int> temp;
        for (int j=0; j < chickens.size(); j++){
            temp.push_back(abs(houses[i].first - chickens[j].first) + abs(houses[i].second - chickens[j].second));
        }
        distances.push_back(temp);
    }
}

int getMin(int a, int b){
    if (a < b){
        return a;
    } else {
        return b;
    }
}

void dfs(int c){
    if (accumulate(check.begin(), check.end(), 0) == m){
        vector<int> temp(houses.size(), 99999);
        for (int i=0; i < houses.size(); i++){
            for (int j=0; j < chickens.size(); j++){
                if (check[j] == 1){
                    temp[i] = getMin(temp[i], distances[i][j]);
                }
            }
        }
        answer = getMin(answer, accumulate(temp.begin(), temp.end(), 0));
        return;
    }

    for (int i=c; i < check.size(); i++){
        check[i] = 1;
        dfs(i+1);
        check[i] = 0;
    }
}


int main(){
    // freopen("sample.txt", "r", stdin);

    input();
    getPosition();
    dfs(0);

    cout << answer << "\n";

    return 0;
}