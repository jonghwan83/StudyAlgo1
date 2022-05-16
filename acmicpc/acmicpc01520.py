import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

m, n = map(int, sys.stdin.readline().split())
graph = []

for _ in range(m):
    graph.append(list(map(int, sys.stdin.readline().split())))

dp = [[-1] * n for _ in range(m)]

def dfs(a, b):
    if dp[a][b] != -1:
        return dp[a][b]
    
    if (a == m-1) and (b == n-1):
        return 1

    dp[a][b] = 0
    
    drows = [-1, 1, 0, 0]
    dcols = [0, 0, -1, 1]

    for i in range(4):
        drow = a + drows[i]
        dcol = b + dcols[i]

        if (0 <= drow < m) and (0 <= dcol < n):
            if graph[a][b] > graph[drow][dcol]:
                dp[a][b] += dfs(drow, dcol)

    return dp[a][b]

dfs(0, 0)
print(dfs(0, 0))