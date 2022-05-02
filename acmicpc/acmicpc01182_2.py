import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

n, s = map(int, sys.stdin.readline().split())
arr = list(map(int, sys.stdin.readline().split()))

answer = 0
def dfs(i, summation):
    global answer
    if i == n:
        return
    if summation + arr[i] == s:
        answer += 1

    dfs(i+1, summation)
    dfs(i+1, summation+arr[i])

dfs(0, 0)
print(answer)
