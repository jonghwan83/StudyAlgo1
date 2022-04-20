import sys

n = int(sys.stdin.readline())
check = [0] * n
answer = sys.maxsize
stat = []

for i in range(n):
    stat.append(list(map(int, sys.stdin.readline().split())))

def dfs(x, p):
    global answer
    if x == n // 2:
        start = 0
        link = 0 
        for i in range(n):
            for j in range(n):
                if check[i] == 1 and check[j] == 1:
                    start += stat[i][j]
                elif check[i] == 0 and check[j] == 0:
                    link += stat[i][j]

        if answer > abs(start - link):
            answer = abs(start - link)
                
        return

    for i in range(p, n):
        check[i] = 1
        dfs(x+1, i+1)
        check[i] = 0


dfs(0, 1)
print(answer)
