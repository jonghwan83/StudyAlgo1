# 시간 오래 걸림

import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

n, s = map(int, sys.stdin.readline().split())
arr = list(map(int, sys.stdin.readline().split()))

check = [0] * len(arr)
answer = 0
def dfs(c, k):
    global answer
    if sum(check) == k:
        temp = 0
        for i in range(len(arr)):
            if check[i] == 1:
                temp += arr[i]
        if temp == s:
            answer += 1
        return

    for i in range(c, len(arr)):
        check[i] = 1
        dfs(i+1, k)
        check[i] = 0


for i in range(1, len(arr) + 1):
    dfs(0, i)

print(answer)
