import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

n, m = map(int, sys.stdin.readline().split())
arr = []    
for i in range(n):
    arr.append(list(map(int, sys.stdin.readline().split())))

locs_chickens = []
locs_houses = []

for  i in range(n):
    for j in range(n):
        if arr[i][j] == 2:
            locs_chickens.append([i, j])
        elif arr[i][j] == 1:
            locs_houses.append([i, j])

distances = []
check = [0] * len(locs_chickens)

for l in locs_houses:
    temp = []
    for k in locs_chickens:
        temp.append(abs(l[0] - k[0]) + abs(l[1] - k[1]))
    distances.append(temp)

ans = sys.maxsize
def dfs(c):
    global ans
    if sum(check) == m:
        sum_of_d = [sys.maxsize] * len(locs_houses)
        for i in range(len(locs_houses)):
            for k in range(len(check)):
                if check[k] == 1:
                    sum_of_d[i] = min(sum_of_d[i], distances[i][k])
        ans = min(ans, sum(sum_of_d))
        return

    for i in range(c, len(check)):
        check[i] = 1
        dfs(i+1)
        check[i] = 0

dfs(0)

print(ans)
