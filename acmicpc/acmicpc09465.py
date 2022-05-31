import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

def solve(n, stickers):
    dp = [[0 for _ in range(n)] for _ in range(2)]
    
    answer = 0
    for j in range(n):
        for i in range(2):
            if j == 0:
                dp[i][j] += stickers[i][j]
            elif j == 1 and i == 0:
                dp[i][j] = dp[i+1][j-1] + stickers[i][j]
            elif j== 1 and i == 1:
                dp[i][j] = dp[i-1][j-1] + stickers[i][j]
            elif j > 1 and i == 0:
                dp[i][j] = max(dp[i+1][j-1], dp[i+1][j-2], dp[i][j-2]) + stickers[i][j]
            elif j > 1 and i == 1:
                dp[i][j] = max(dp[i-1][j-1], dp[i-1][j-2], dp[i][j-2]) + stickers[i][j]

            if dp[i][j] > answer:
                answer = dp[i][j]
    
    return answer


T = int(sys.stdin.readline())
for _ in range(T):
    n = int(sys.stdin.readline())
    stickers = []
    for _ in range(2):
        stickers.append(list(map(int, sys.stdin.readline().split())))

    print(solve(n, stickers))

