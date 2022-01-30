#include <stdio.h>

using namespace std;
int main(){
    unsigned int n;
    scanf("%d", &n);

    for (int i=0; i < n; i++){
        char parenthesis[50];
        scanf("%s", parenthesis);

        int chk;
        chk = 0;

        for (int i=0; parenthesis[i]; i++){
            if (chk < 0){
                break;
            }
            if (parenthesis[i] == '('){
                chk += 1;
            } else {
                chk -= 1;
            }
        }
        if (chk == 0){
            printf("%s\n", "YES");
        } else {
            printf("%s\n", "NO");
        }
    }
}